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
	void save(std::string* pString);
	void load(const std::string& string);
	int size();
	virtual void compute(cv::Mat& image, cv::Mat& mask)=0;
	/// By default GrabCut is used for segmentation.
	virtual void segment(cv::Mat* pMask, cv::Mat& image);
	float norm();
	void normalize();
protected:
	std::vector<float> mVector;
};

#endif /* FEATURE_H_ */
