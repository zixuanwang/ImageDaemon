/*
 * ColorFeature.h
 *
 *  Created on: Jan 21, 2012
 *      Author: zixuan
 */

#ifndef COLORFEATURE_H_
#define COLORFEATURE_H_
#include "Feature.h"
class ColorFeature: public Feature {
public:
	ColorFeature();
	virtual ~ColorFeature();
	virtual void compute(cv::Mat& image, cv::Mat& mask);
};

#endif /* COLORFEATURE_H_ */
