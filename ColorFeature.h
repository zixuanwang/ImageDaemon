/*
 * ColorFeature.h
 *
 *  Created on: Jan 21, 2012
 *      Author: zixuan
 */

#ifndef COLORFEATURE_H_
#define COLORFEATURE_H_
#include "Feature.h"
#include "HbaseAdapter.h"
class ColorFeature: public Feature {
public:
	ColorFeature(int channelbins);
	virtual ~ColorFeature();
	virtual void load(int64_t rowKey);
	virtual void compute(cv::Mat& image, cv::Mat& mask);
private:
	int mChannelbins;
};

#endif /* COLORFEATURE_H_ */
