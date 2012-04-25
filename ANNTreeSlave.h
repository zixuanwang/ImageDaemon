/*
 * ANNTreeSlave.h
 *
 *  Created on: Feb 20, 2012
 *      Author: zixuan
 */

#ifndef ANNTREESLAVE_H_
#define ANNTREESLAVE_H_
#include "GlobalConfig.h"
#include "Serializer.h"
class ANNTreeSlave {
public:
	ANNTreeSlave(int treeIndex);
	~ANNTreeSlave();
	void addFeature(int64_t id, const std::vector<float>& feature);
	/// After index, should not add feature any more.
	void index();
	void knnSearch(std::vector<int64_t>* pNeighborIdArray,
			std::vector<float>* pDistArray, const std::vector<float>& feature,
			int k);
	void save();
	void load();
private:
	int mTreeIndex;
	cv::Mat mMat;
	std::vector<float> mData;
	boost::unordered_map<int, int64_t> mIdMap;
	int mFeatureCount;
	boost::shared_ptr<cv::flann::Index> mpIndex;
	boost::mutex mMutex;
};

#endif /* ANNTREESLAVE_H_ */
