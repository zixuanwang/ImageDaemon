/*
 * ANNTreeSlave.cpp
 *
 *  Created on: Feb 20, 2012
 *      Author: zixuan
 */

#include "ANNTreeSlave.h"

ANNTreeSlave::ANNTreeSlave(int treeIndex) :
		mTreeIndex(treeIndex), mFeatureCount(0) {
}

ANNTreeSlave::~ANNTreeSlave() {

}

void ANNTreeSlave::addFeature(int64_t id, const std::vector<float>& feature) {
	boost::mutex::scoped_lock lock(mMutex);
	std::copy(feature.begin(), feature.end(), std::back_inserter(mData));
	mIdMap[mFeatureCount] = id;
	++mFeatureCount;
}

void ANNTreeSlave::index() {
	boost::mutex::scoped_lock lock(mMutex);
	if (mData.empty() || mFeatureCount == 0) {
		return;
	}
	mMat = cv::Mat(mData, false);
	mMat = mMat.reshape(0, mFeatureCount);
	mpIndex.reset(new cv::flann::Index(mMat, cv::flann::KDTreeIndexParams(8)));
}

void ANNTreeSlave::knnSearch(std::vector<int64_t>* pNeighborIdArray,
		std::vector<float>* pDistArray, const std::vector<float>& feature,
		int k) {
	pNeighborIdArray->clear();
	pDistArray->clear();
	if (mFeatureCount == 0) {
		return;
	}
	k = k < mFeatureCount ? k : mFeatureCount;
	std::vector<int> indices(k);
	std::vector<float> dists(k);
	mpIndex->knnSearch(feature, indices, dists, k, cv::flann::SearchParams(64));
	pDistArray->assign(dists.begin(), dists.end());
	pNeighborIdArray->reserve(indices.size());
	for (size_t i = 0; i < indices.size(); ++i) {
		pNeighborIdArray->push_back(mIdMap[indices[i]]);
	}
}
