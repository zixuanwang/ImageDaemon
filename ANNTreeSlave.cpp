/*
 * ANNTreeSlave.cpp
 *
 *  Created on: Feb 20, 2012
 *      Author: zixuan
 */

#include "ANNTreeSlave.h"
ANNTreeSlave* ANNTreeSlave::pInstance = NULL;
pthread_mutex_t ANNTreeSlave::sMutex = PTHREAD_MUTEX_INITIALIZER;

ANNTreeSlave::ANNTreeSlave() :
		mFeatureCount(0) {
}

ANNTreeSlave* ANNTreeSlave::instance() {
	if (pInstance == NULL) {
		pInstance = new ANNTreeSlave;
	}
	return pInstance;
}

void ANNTreeSlave::init() {
	pthread_mutex_lock(&sMutex);
	mFeatureCount = 0;
	mData.clear();
	mIdMap.clear();
	pthread_mutex_unlock(&sMutex);
}

void ANNTreeSlave::addFeature(int64_t id, const std::vector<float>& feature) {
	pthread_mutex_lock(&sMutex);
	std::copy(feature.begin(), feature.end(), std::back_inserter(mData));
	mIdMap[mFeatureCount] = id;
	++mFeatureCount;
	pthread_mutex_unlock(&sMutex);
}

void ANNTreeSlave::index() {
	pthread_mutex_lock(&sMutex);
	mMat = cv::Mat(mData, false);
	mMat = mMat.reshape(0, mFeatureCount);
	mpIndex.reset(new cv::flann::Index(mMat, cv::flann::KDTreeIndexParams(8)));
	pthread_mutex_unlock(&sMutex);
}

void ANNTreeSlave::knnSearch(std::vector<int64_t>* pNeighborIdArray,
		std::vector<float>* pDistArray, const std::vector<float>& feature,
		int k) {
	pNeighborIdArray->clear();
	pDistArray->clear();
	std::vector<int> indices(k);
	std::vector<float> dists(k);
	mpIndex->knnSearch(feature, indices, dists, k, cv::flann::SearchParams(64));
	pDistArray->assign(dists.begin(), dists.end());
	pNeighborIdArray->reserve(indices.size());
	for (size_t i = 0; i < indices.size(); ++i) {
		pNeighborIdArray->push_back(mIdMap[indices[i]]);
	}
}

int64_t ANNTreeSlave::featureCount(){
	return mFeatureCount;
}
