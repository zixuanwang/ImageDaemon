/*
 * InvertedIndex.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#include "InvertedIndex.h"

InvertedIndex* InvertedIndex::pInstance = NULL;
pthread_mutex_t InvertedIndex::sMutex = PTHREAD_MUTEX_INITIALIZER;

InvertedIndex* InvertedIndex::instance() {
	if (pInstance == NULL) {
		pInstance = new InvertedIndex;
	}
	return pInstance;
}

InvertedIndex::InvertedIndex() {
}

void InvertedIndex::init() {
	// assign empty invert index
	std::multiset<Posting, PostingComparator> dummySet;
	mInvertIndex.assign(GlobalConfig::VOCABULARY_SIZE, dummySet);
	// read the short invert index from the hbase, this may take some time
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	std::vector<std::string> columnNameArray;
	columnNameArray.push_back(GlobalConfig::COLUMN_FAMILY);
	int scanner = dbAdapter->scannerOpen(GlobalConfig::INVERT_INDEX_TABLE, "",
			columnNameArray);
	while (true) {
		std::string strRowKey;
		std::map<std::string, std::string> postingMap;
		dbAdapter->scannerGet(&strRowKey, &postingMap, scanner);
		if (strRowKey.empty()) {
			break;
		}
		int64_t visualwordId = 0;
		TypeConverter::readStringToNum(&visualwordId, strRowKey);
		std::string strNextColumnId =
				postingMap[GlobalConfig::INVERT_INDEX_NEXT_COLUMN];
		int64_t nextColumnId = 0;
		TypeConverter::readStringToNum(&nextColumnId, strNextColumnId);
		std::vector<Posting> postingArray;
		postingArray.reserve(nextColumnId);
		for (int64_t i = 0; i < nextColumnId; ++i) {
			std::string columnName = GlobalConfig::COLUMN_FAMILY
					+ boost::lexical_cast<std::string>(i);
			std::string strPosting = postingMap[columnName];
			if (strPosting.empty()) {
				continue;
			}
			Posting posting;
			memcpy((char*) &(posting.imageId), (char*) &strPosting[0],
					sizeof(posting.imageId));
			memcpy((char*) &(posting.score),
					(char*) &strPosting[sizeof(posting.imageId)],
					sizeof(posting.score));
			postingArray.push_back(posting);
		}
		// only consider top k inverted index
		if ((int) postingArray.size() < GlobalConfig::SHORT_LIST_SIZE) {
			std::sort(postingArray.begin(), postingArray.end(),
					PostingComparator());
		} else {
			std::partial_sort(postingArray.begin(),
					postingArray.begin() + GlobalConfig::SHORT_LIST_SIZE,
					postingArray.end(), PostingComparator());
		}
		// add the champion list to the memory
		std::multiset<Posting, PostingComparator>& postingSet =
				mInvertIndex[visualwordId];
		for (int i = 0;
				i < GlobalConfig::SHORT_LIST_SIZE
						&& i < (int) postingArray.size(); ++i) {
			postingSet.insert(postingArray[i]);
		}
	}
	dbAdapter->scannerClose(scanner);
}

void InvertedIndex::savePostingList(int64_t visualwordId,
		const std::vector<Posting>& postingArray) {
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	// write to hbase
	std::string strVisualwordId;
	TypeConverter::readNumToString(&strVisualwordId, visualwordId);
	int64_t size = (int64_t) postingArray.size();
	std::string strSize;
	TypeConverter::readNumToString(&strSize, size);
	dbAdapter->saveCell(strSize, GlobalConfig::INVERT_INDEX_TABLE,
			strVisualwordId, GlobalConfig::INVERT_INDEX_NEXT_COLUMN);
	for (size_t i = 0; i < postingArray.size(); ++i) {
		std::string columnName = GlobalConfig::COLUMN_FAMILY
				+ boost::lexical_cast<std::string>(i);
		savePosting(strVisualwordId, columnName, postingArray[i]);
	}
	std::vector<Posting> postingArrayCpy(postingArray);
	std::vector<Posting>::iterator endIter;
	if ((int) postingArrayCpy.size() < GlobalConfig::SHORT_LIST_SIZE) {
		std::sort(postingArrayCpy.begin(), postingArrayCpy.end(),
				PostingComparator());
		endIter = postingArrayCpy.end();
	} else {
		std::partial_sort(postingArrayCpy.begin(),
				postingArrayCpy.begin() + GlobalConfig::SHORT_LIST_SIZE,
				postingArrayCpy.end(), PostingComparator());
		endIter = postingArrayCpy.begin() + GlobalConfig::SHORT_LIST_SIZE;
	}
	std::vector<Posting> championArray(postingArrayCpy.begin(), endIter);
	updateChampionList(visualwordId, championArray);
}

void InvertedIndex::loadPostingList(std::vector<Posting>* pPostingArray,
		int64_t visualwordId) {
	pPostingArray->assign(mInvertIndex[visualwordId].begin(),
			mInvertIndex[visualwordId].end());
}

void InvertedIndex::addPosting(int64_t visualwordId, const Posting& posting) {
	// write to Hbase
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	std::string strNextColumnId;
	std::string strVisualwordId;
	TypeConverter::readNumToString(&strVisualwordId, visualwordId);
	dbAdapter->loadCell(&strNextColumnId, GlobalConfig::INVERT_INDEX_TABLE,
			strVisualwordId, GlobalConfig::INVERT_INDEX_NEXT_COLUMN);
	int64_t nextColumnId = 0;
	TypeConverter::readStringToNum(&nextColumnId, strNextColumnId);
	std::string nextColumnName = GlobalConfig::COLUMN_FAMILY
			+ boost::lexical_cast<std::string>(nextColumnId);
	savePosting(strVisualwordId, nextColumnName, posting);
	// update the next id
	++nextColumnId;
	TypeConverter::readNumToString(&strNextColumnId, nextColumnId);
	dbAdapter->saveCell(strNextColumnId, GlobalConfig::INVERT_INDEX_TABLE,
			strVisualwordId, GlobalConfig::INVERT_INDEX_NEXT_COLUMN);
	std::multiset<Posting, PostingComparator>& postingSet =
			mInvertIndex[visualwordId];
	std::multiset<Posting, PostingComparator>::reverse_iterator riter =
			postingSet.rbegin();
	if (riter->score >= posting.score) {
		// enter the critical region
		pthread_mutex_lock(&sMutex);
		// add to the memory
		postingSet.insert(posting);
		riter = postingSet.rbegin();
		postingSet.erase(--(riter.base()));
		pthread_mutex_unlock(&sMutex);
	}
}

void InvertedIndex::updateChampionList(int64_t visualwordId,
		const std::vector<Posting>& postingArray) {
	std::multiset<Posting, PostingComparator>& postingSet =
			mInvertIndex[visualwordId];
	pthread_mutex_lock(&sMutex);
	postingSet.clear();
	postingSet.insert(postingArray.begin(), postingArray.end());
	pthread_mutex_unlock(&sMutex);
}

void InvertedIndex::savePosting(const std::string& strVisualwordId,
		const std::string& columnName, const Posting& posting) {
	// write to Hbase
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	std::string strPosting(16, '0');
	memcpy((char*) &strPosting[0], (char*) &(posting.imageId),
			sizeof(posting.imageId));
	memcpy((char*) &strPosting[sizeof(posting.imageId)],
			(char*) &(posting.score), sizeof(posting.score));
	dbAdapter->saveCell(strPosting, GlobalConfig::INVERT_INDEX_TABLE,
			strVisualwordId, columnName);
}
