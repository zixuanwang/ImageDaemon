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
#include "ImageResizer.h"
#include "Image.h"
class Feature {
public:
	Feature();
	virtual ~Feature();
	virtual void load(int64_t rowKey) = 0;
	virtual void save(int64_t rowKey) = 0;
	virtual void compute(const cv::Mat& image);
	virtual void compute(int64_t imageKey);
	virtual void compute(const cv::Mat& image, const cv::Mat& mask) = 0;
	void save(std::string* pString);
	void load(const std::string& string);
	float norm();
	void normalize();
	int size();
	bool empty();
	virtual void add(const boost::shared_ptr<ANNTreeRoot>& pRoot,
			int64_t imageId);
	virtual void knnSearch(std::vector<Neighbor>* pNeighborArray,
			boost::shared_ptr<ANNTreeRoot> pRoot, int k);
	const std::vector<float>& getFeature();
	virtual void getFeature(std::vector<float>* pFeature, int index);
	/// By default GrabCut is used for segmentation.
	void segment(cv::Mat* pMask, const cv::Mat& image);
protected:
	std::vector<float> mVector;
};

#endif /* FEATURE_H_ */
