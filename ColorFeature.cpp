/*
 * ColorFeature.cpp
 *
 *  Created on: Jan 21, 2012
 *      Author: zixuan
 */

#include "ColorFeature.h"

ColorFeature::ColorFeature(int channelbins) :
		Feature(), mChannelbins(channelbins) {

}

ColorFeature::~ColorFeature() {

}

void ColorFeature::load(int64_t rowKey) {
	mVector.clear();
	std::string strVector;
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	dbAdapter->loadCell(&strVector, GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_COLOR_FEATURE_COLUMN);
	Feature::load(strVector);
}

void ColorFeature::compute(cv::Mat& image, cv::Mat& mask) {
	mVector.clear();
	if (image.empty()) {
		return;
	}
	int histSize[] = { mChannelbins, mChannelbins, mChannelbins };
	float hranges[] = { 0.0, 255.0 };
	const float* ranges[] = { hranges, hranges, hranges };
	int channels[] = { 0, 1, 2 };
	cv::MatND hist;
	cv::calcHist(&image, 1, channels, mask, hist, 3, histSize, ranges);
	mVector.reserve(mChannelbins * mChannelbins * mChannelbins);
	for (int i = 0; i < mChannelbins; ++i) {
		for (int j = 0; j < mChannelbins; ++j) {
			for (int k = 0; k < mChannelbins; ++k) {
				float value = hist.at<float>(i, j, k);
				mVector.push_back(value);
			}
		}
	}
	normalize();
}
