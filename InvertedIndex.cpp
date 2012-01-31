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

void InvertedIndex::init() {
	// load the short posting list by default
	int64_t vocabularySize = ANNVocabulary::instance()->size();
	std::vector<int64_t> dummyID;
	mImageIDArray.assign(vocabularySize, dummyID);
	std::vector<double> dummyScore;
	mImageScoreArray.assign(vocabularySize, dummyScore);
	std::vector<str::string> columnNameArray;
	columnNameArray.push_back(GlobalConfig::INVERT_INDEX_SHORT_COLUMN);
	columnNameArray.push_back(GlobalConfig::INVERT_INDEX_SHORT_SCORE_COLUMN);
	for (int64_t i = 0; i < vocabularySize; ++i) {
		std::vector<std::string> shortPostingArray;
		MongoDBAdapter::instance()->loadCells(&shortPostingArray,
				GlobalConfig::INVERT_INDEX_TABLE, i, columnNameArray);
		if (shortPostingArray.size() == 2) {
			std::vector<int64_t>& imageID = mImageIDArray[i];
			imageID.assign(shortPostingArray[0].size() / sizeof(int64_t), 0);
			if (!shortPostingArray[0].empty()) {
				memcpy((char*) &imageID[0], (char*) &shortPostingArray[0][0],
						shortPostingArray[0].size());
			}
			std::vector<double>& imageScore = mImageScoreArray[i];
			imageScore.assign(shortPostingArray[1].size() / sizeof(double),
					0.0);
			if (!shortPostingArray[1].empty()) {
				memcpy((char*) &imageScore[0], (char*) &shortPostingArray[1][0],
						shortPostingArray[1].size());
			}
		}
	}
}

void InvertedIndex::savePostingList(int64_t visualwordID,
		const std::vector<Posting>& postingArray) {
	if (postingArray.empty()) {
		return;
	}
	// save the posting list ordered by scores
	// save them to two columns
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
	MongoDBAdapter::instance()->saveCell(shortId.str(),
			GlobalConfig::INVERT_INDEX_TABLE, visualwordID,
			GlobalConfig::INVERT_INDEX_SHORT_COLUMN);
	MongoDBAdapter::instance()->saveCell(shortScore.str(),
			GlobalConfig::INVERT_INDEX_TABLE, visualwordID,
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
	MongoDBAdapter::instance()->saveCell(fullId.str(),
			GlobalConfig::INVERT_INDEX_TABLE, visualwordID,
			GlobalConfig::INVERT_INDEX_LONG_COLUMN);
	MongoDBAdapter::instance()->saveCell(fullScore.str(),
			GlobalConfig::INVERT_INDEX_TABLE, visualwordID,
			GlobalConfig::INVERT_INDEX_LONG_SCORE_COLUMN);
}

void InvertedIndex::loadPostingList(std::vector<int64_t>* pImageIDArray,
		std::vector<double>* pImageScoreArray, int64_t visualwordID) {
	// return the inverted index from memory
	pImageIDArray->assign(mImageIDArray[visualwordID].begin(),
			mImageIDArray[visualwordID].end());
	pImageScoreArray->assign(mImageScoreArray[visualwordID].begin(),
			mImageScoreArray[visualwordID].end());

//	std::vector<std::string> shortPostingArray;
//	std::vector<str::string> columnNameArray;
//	columnNameArray.push_back(GlobalConfig::INVERT_INDEX_SHORT_COLUMN);
//	columnNameArray.push_back(GlobalConfig::INVERT_INDEX_SHORT_SCORE_COLUMN);
//	MongoDBAdapter::instance()->loadCells(&shortPostingArray,
//			GlobalConfig::INVERT_INDEX_TABLE, visualwordID, columnNameArray);
//	if (shortPostingArray.size() == 2) {
//		pImageIDArray->assign(shortPostingArray[0].size() / sizeof(int64_t), 0);
//		if (!shortPostingArray[0].empty()) {
//			memcpy((char*) &(*pImageIDArray)[0],
//					(char*) &shortPostingArray[0][0],
//					shortPostingArray[0].size());
//		}
//		pImageScoreArray->assign(shortPostingArray[1].size() / sizeof(double),
//				0.0);
//		if (!shortPostingArray[1].empty()) {
//			memcpy((char*) &(*pImageScoreArray)[0],
//					(char*) &shortPostingArray[1][0],
//					shortPostingArray[1].size());
//		}
//	}
}
