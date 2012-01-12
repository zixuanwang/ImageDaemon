/*
 * InvertedIndex.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#include "InvertedIndex.h"

InvertedIndex* InvertedIndex::pInstance = NULL;

InvertedIndex* InvertedIndex::instance() {
	if (pInstance == NULL) {
		pInstance = new InvertedIndex;
	}
	return pInstance;
}

InvertedIndex::InvertedIndex() {

}

void InvertedIndex::savePostingList(int64_t visualwordID,
		const std::vector<Posting>& postingArray) {
	// save the posting list ordered by scores
	// save them to two columns
	std::string strRowKey;
	TypeConverter::readNumToString(&strRowKey, visualwordID);
	std::vector<Posting> postingArrayCpy(postingArray);
	std::sort(postingArrayCpy.begin(), postingArrayCpy.end(),
			PostingComparator());
	// save the short posting list
	std::stringstream shortId(
			std::stringstream::out | std::stringstream::binary);
	std::stringstream shortScore(
			std::stringstream::out | std::stringstream::binary);
	for (int i = 0;
			i < GlobalConfig::SHORT_LIST_SIZE
					&& i < (int) postingArrayCpy.size(); ++i) {
		std::string string;
		TypeConverter::readNumToString(&string, postingArrayCpy[i].imageID);
		shortId.write(&string[0], string.size());
		TypeConverter::readNumToString(&string, postingArrayCpy[i].score);
		shortScore.write(&string[0], string.size());
	}
	HbaseAdapter::instance()->saveCell(shortId.str(),
			GlobalConfig::INVERT_INDEX_TABLE, strRowKey,
			GlobalConfig::INVERT_INDEX_SHORT_COLUMN);
	HbaseAdapter::instance()->saveCell(shortScore.str(),
			GlobalConfig::INVERT_INDEX_TABLE, strRowKey,
			GlobalConfig::INVERT_INDEX_SHORT_SCORE_COLUMN);
	// save the full posting list
	std::stringstream fullId(
			std::stringstream::out | std::stringstream::binary);
	std::stringstream fullScore(
			std::stringstream::out | std::stringstream::binary);
	for (size_t i = 0; i < postingArrayCpy.size(); ++i) {
		std::string string;
		TypeConverter::readNumToString(&string, postingArrayCpy[i].imageID);
		fullId.write(&string[0], string.size());
		TypeConverter::readNumToString(&string, postingArrayCpy[i].score);
		fullScore.write(&string[0], string.size());
	}
	HbaseAdapter::instance()->saveCell(fullId.str(),
			GlobalConfig::INVERT_INDEX_TABLE, strRowKey,
			GlobalConfig::INVERT_INDEX_LONG_COLUMN);
	HbaseAdapter::instance()->saveCell(fullScore.str(),
			GlobalConfig::INVERT_INDEX_TABLE, strRowKey,
			GlobalConfig::INVERT_INDEX_LONG_SCORE_COLUMN);
}

void InvertedIndex::loadPostingList(std::vector<int64_t>* imageIDArray,
		int64_t visualwordID) {
	// load the short posting list by default
	std::string strRowKey;
	TypeConverter::readNumToString(&strRowKey, visualwordID);
	std::string strShortPostingList;
	HbaseAdapter::instance()->loadCell(&strShortPostingList,
			GlobalConfig::INVERT_INDEX_TABLE, strRowKey,
			GlobalConfig::INVERT_INDEX_SHORT_COLUMN);
	imageIDArray->assign(strShortPostingList.size() / sizeof(int64_t), 0);
	memcpy((char*) &(*imageIDArray)[0], (char*) &strShortPostingList[0],
			strShortPostingList.size());
}
