/*
 * ShapeFeature.h
 *
 *  Created on: Mar 17, 2012
 *      Author: zixuan
 */

#ifndef SHAPEFEATURE_H_
#define SHAPEFEATURE_H_
#include "GlobalConfig.h"
#include "Feature.h"
#include "HbaseAdapter.h"
#include "ImageResizer.h"
class ShapeFeature: public Feature {
public:
	ShapeFeature(int rbins, int abins);
	virtual ~ShapeFeature();
	virtual void load(int64_t rowKey);
	virtual void save(int64_t rowKey);
	virtual void compute(const cv::Mat& image, const cv::Mat& mask);
private:
	int mRbins; //radius bins
	int mAbins; //angle bins
};

#endif /* SHAPEFEATURE_H_ */
