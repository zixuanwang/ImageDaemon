/*
 * ANNVocabulary.cpp
 *
 *  Created on: May 12, 2011
 *      Author: zxwang
 */

#include "ANNVocabulary.h"

const int ANNVocabulary::SOFT_ASSIGN = 1;
ANNVocabulary* ANNVocabulary::pInstance = NULL;

ANNVocabulary::ANNVocabulary() {
}

ANNVocabulary* ANNVocabulary::instance() {
	if (pInstance == NULL) {
		pInstance = new ANNVocabulary;
	}
	return pInstance;
}

void ANNVocabulary::init(const std::string& vocabularyPath) {
	std::ifstream inStream;
	inStream.open(vocabularyPath.c_str(), std::ios::binary);
	if (inStream.good()) {
		int rows, cols, bytes;
		inStream.read((char*) &rows, sizeof(rows));
		inStream.read((char*) &cols, sizeof(cols));
		inStream.read((char*) &bytes, sizeof(bytes));
		mVocabularyMat = cv::Mat(rows, cols, CV_32FC1);
		inStream.read((char*) mVocabularyMat.data, bytes);
		mpVocabularyIndex.reset(
				new cv::flann::Index(mVocabularyMat,
						cv::flann::KDTreeIndexParams(8)));
		inStream.close();
	}
}

void ANNVocabulary::quantizeFeature(const LocalFeature& localFeature,
		BoWHistogram* pHistogram) {
	pHistogram->clear();
	if (localFeature.empty()) {
		return;
	}
	int featureCount = localFeature.descriptor.rows;
	cv::Mat index(featureCount, SOFT_ASSIGN, CV_32SC1);
	cv::Mat dist(featureCount, SOFT_ASSIGN, CV_32FC1);
	mpVocabularyIndex->knnSearch(localFeature.descriptor, index, dist,
			SOFT_ASSIGN, cv::flann::SearchParams(64));
	boost::unordered_map<int64_t, double> visualWordHit;
	for (int i = 0; i < featureCount; ++i) {
		int *ptr = index.ptr<int>(i);
		for (int j = 0; j < SOFT_ASSIGN; ++j) {
			visualWordHit[(int64_t) ptr[j]] += 1.0;
		}
	}
	for (boost::unordered_map<int64_t, double>::iterator iter =
			visualWordHit.begin(); iter != visualWordHit.end(); ++iter) {
		pHistogram->addBin(iter->first, 1 + log(iter->second));
	}
	pHistogram->normalize();
}
