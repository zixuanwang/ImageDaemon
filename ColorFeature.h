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
#include "ImageResizer.h"
class ColorFeature: public Feature {
public:
	ColorFeature(int channelbins);
	virtual ~ColorFeature();
	virtual void load(int64_t rowKey);
	virtual void save(int64_t rowKey);
	virtual void compute(const cv::Mat& image);
private:
	int mChannelbins;
};

#endif /* COLORFEATURE_H_ */
