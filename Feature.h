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
class Feature {
public:
	Feature();
	virtual ~Feature();
	virtual void load(int64_t rowKey)=0;
	void save(std::string* pString);
	void load(const std::string& string);
	int size();
	bool empty();
	virtual void compute(cv::Mat& image, cv::Mat& mask)=0;
	/// By default GrabCut is used for segmentation.
	void segment(cv::Mat& mask, cv::Mat& image);
	float norm();
	void normalize();
	const std::vector<float>& getFeature();
protected:
	std::vector<float> mVector;
};

#endif /* FEATURE_H_ */
