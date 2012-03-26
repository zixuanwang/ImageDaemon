/*
 * ANNTreeSlave.h
 *
 *  Created on: Feb 20, 2012
 *      Author: zixuan
 */

#ifndef ANNTREESLAVE_H_
#define ANNTREESLAVE_H_
#include <opencv2/opencv.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
class ANNTreeSlave {
public:
	static ANNTreeSlave* instance();
	/// Clear all data
	void clear();
	void init(int treeIndex);
	int addTree();
	int treeCount();
	void addFeature(int treeIndex, int64_t id, const std::vector<float>& feature);
	void index(int treeIndex);
	void knnSearch(std::vector<int64_t>* pNeighborIdArray,
			std::vector<float>* pDistArray, int treeIndex, const std::vector<float>& feature,
			int k);
	int featureCount(int treeIndex);
private:
	std::vector<cv::Mat> mMatArray;
	std::vector<std::vector<float> > mDataArray;
	std::vector<boost::unordered_map<int, int64_t> > mIdMapArray;
	std::vector<int> mFeatureCountArray;
	std::vector<boost::shared_ptr<cv::flann::Index> > mpIndexArray;
	static ANNTreeSlave* pInstance;
	ANNTreeSlave();
	ANNTreeSlave(const ANNTreeSlave&);
	ANNTreeSlave& operator=(const ANNTreeSlave&);
	static pthread_mutex_t sMutex;
};

#endif /* ANNTREESLAVE_H_ */
