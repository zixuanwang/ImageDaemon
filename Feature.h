/*
 * Feature.h
 *
 *  Created on: Jan 21, 2012
 *      Author: zixuan
 */

#ifndef FEATURE_H_
#define FEATURE_H_
#include "GlobalConfig.h"
#include "TypeConverter.h"
#include "ANNTreeRoot.h"
class Feature {
public:
	Feature();
	virtual ~Feature();
	virtual void load(int64_t rowKey) = 0;
	virtual void save(int64_t rowKey) = 0;
	virtual void compute(const cv::Mat& image) = 0;
	void save(std::string* pString);
	void load(const std::string& string);
	float norm();
	void normalize();
	int size();
	bool empty();
	virtual void add(boost::shared_ptr<ANNTreeRoot> pRoot,
			int64_t imageId);
	virtual void knnSearch(std::vector<Neighbor>* pNeighborArray,
			boost::shared_ptr<ANNTreeRoot> pRoot, int k);
	const std::vector<float>& getFeature();
	virtual void getFeature(std::vector<float>* pFeature,
				int index);
protected:
	std::vector<float> mVector;
	/// By default GrabCut is used for segmentation.
	virtual void segment(cv::Mat* pMask, const cv::Mat& image);
};

#endif /* FEATURE_H_ */
