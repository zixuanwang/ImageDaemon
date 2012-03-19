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
	void init();
	void addFeature(int64_t id, const std::vector<float>& feature);
	void index();
	void knnSearch(std::vector<int64_t>* pNeighborIdArray,
			std::vector<float>* pDistArray, const std::vector<float>& feature,
			int k);
	int64_t featureCount();
private:
	cv::Mat mMat;
	std::vector<float> mData;
	boost::unordered_map<int, int64_t> mIdMap;
	int64_t mFeatureCount;
	boost::shared_ptr<cv::flann::Index> mpIndex;
	static ANNTreeSlave* pInstance;
	// Default constuctor, copy constructor and assignment operators should be as private so that nobody
	// from outside can call those functions and instantiate it
	ANNTreeSlave();
	ANNTreeSlave(const ANNTreeSlave&);
	ANNTreeSlave& operator=(const ANNTreeSlave&);
	static pthread_mutex_t sMutex;
};

#endif /* ANNTREESLAVE_H_ */
