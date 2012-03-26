/*
 * ANNTreeSlave.cpp
 *
 *  Created on: Feb 20, 2012
 *      Author: zixuan
 */

#include "ANNTreeSlave.h"
ANNTreeSlave* ANNTreeSlave::pInstance = NULL;
pthread_mutex_t ANNTreeSlave::sMutex = PTHREAD_MUTEX_INITIALIZER;

ANNTreeSlave::ANNTreeSlave() {
}

ANNTreeSlave* ANNTreeSlave::instance() {
	if (pInstance == NULL) {
		pInstance = new ANNTreeSlave;
	}
	return pInstance;
}

void ANNTreeSlave::clear() {
	pthread_mutex_lock(&sMutex);
	mMatArray.clear();
	mDataArray.clear();
	mIdMapArray.clear();
	mFeatureCountArray.clear();
	mpIndexArray.clear();
	pthread_mutex_unlock(&sMutex);
}

void ANNTreeSlave::init(int treeIndex) {
	if (treeIndex >= treeCount()) {
		std::cerr << "Wrong treeIndex." << std::endl;
		return;
	}
	pthread_mutex_lock(&sMutex);
	mMatArray[treeIndex] = cv::Mat();
	mDataArray[treeIndex].clear();
	mIdMapArray[treeIndex].clear();
	mFeatureCountArray[treeIndex] = 0;
	mpIndexArray[treeIndex].reset();
	pthread_mutex_unlock(&sMutex);
}

int ANNTreeSlave::addTree() {
	int count = treeCount() + 1;
	pthread_mutex_lock(&sMutex);
	mMatArray.resize(count);
	mDataArray.resize(count);
	mIdMapArray.resize(count);
	mFeatureCountArray.resize(count);
	mpIndexArray.resize(count);
	pthread_mutex_unlock(&sMutex);
	return count - 1;
}

int ANNTreeSlave::treeCount() {
	return (int) mMatArray.size();
}

void ANNTreeSlave::addFeature(int treeIndex, int64_t id,
		const std::vector<float>& feature) {
	if (treeIndex >= treeCount()) {
		std::cerr << "Wrong treeIndex." << std::endl;
		return;
	}
	pthread_mutex_lock(&sMutex);
	std::copy(feature.begin(), feature.end(),
			std::back_inserter(mDataArray[treeIndex]));
	mIdMapArray[treeIndex][mFeatureCountArray[treeIndex]] = id;
	++mFeatureCountArray[treeIndex];
	pthread_mutex_unlock(&sMutex);
}

void ANNTreeSlave::index(int treeIndex) {
	if (treeIndex >= treeCount()) {
		std::cerr << "Wrong treeIndex." << std::endl;
		return;
	}
	if (mDataArray[treeIndex].empty() || mFeatureCountArray[treeIndex] == 0) {
		return;
	}
	pthread_mutex_lock(&sMutex);
	mMatArray[treeIndex] = cv::Mat(mDataArray[treeIndex], false);
	mMatArray[treeIndex] = mMatArray[treeIndex].reshape(0,
			mFeatureCountArray[treeIndex]);
	mpIndexArray[treeIndex].reset(
			new cv::flann::Index(mMatArray[treeIndex],
					cv::flann::KDTreeIndexParams(8)));
	pthread_mutex_unlock(&sMutex);
}

void ANNTreeSlave::knnSearch(std::vector<int64_t>* pNeighborIdArray,
		std::vector<float>* pDistArray, int treeIndex,
		const std::vector<float>& feature, int k) {
	if (treeIndex >= treeCount()) {
		std::cerr << "Wrong treeIndex." << std::endl;
		return;
	}
	pNeighborIdArray->clear();
	pDistArray->clear();
	if (mFeatureCountArray[treeIndex] == 0) {
		return;
	}
	k = k < mFeatureCountArray[treeIndex] ? k : mFeatureCountArray[treeIndex];
	std::vector<int> indices(k);
	std::vector<float> dists(k);
	mpIndexArray[treeIndex]->knnSearch(feature, indices, dists, k,
			cv::flann::SearchParams(64));
	pDistArray->assign(dists.begin(), dists.end());
	pNeighborIdArray->reserve(indices.size());
	boost::unordered_map<int, int64_t>& idMap = mIdMapArray[treeIndex];
	for (size_t i = 0; i < indices.size(); ++i) {
		pNeighborIdArray->push_back(idMap[indices[i]]);
	}
}

int ANNTreeSlave::featureCount(int treeIndex) {
	if (treeIndex >= treeCount()) {
		std::cerr << "Wrong treeIndex." << std::endl;
		return 0;
	}
	return mFeatureCountArray[treeIndex];
}
