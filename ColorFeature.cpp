/*
 * ColorFeature.cpp
 *
 *  Created on: Jan 21, 2012
 *      Author: zixuan
 */

#include "ColorFeature.h"

ColorFeature::ColorFeature() {

}

ColorFeature::~ColorFeature() {
}

void ColorFeature::compute(cv::Mat& image, cv::Mat& mask) {
	mVector.clear();
	if (image.empty()) {
		return;
	}
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);
	// let's quantize the hue to 10 levels
	// and the saturation to 10 levels
	int hbins = 10, sbins = 10;
	int histSize[] = { hbins, sbins };
	// hue varies from 0 to 179, see cvtColor
	float hranges[] = { 0, 180 };
	// saturation varies from 0 (black-gray-white) to
	// 255 (pure spectrum color)
	float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges, sranges };
	cv::MatND hist;
	int channels[] = { 0, 1 };
	cv::calcHist(&hsv, 1, channels, mask, hist, 2, histSize, ranges, true,
			false);
	mVector.reserve(hbins * sbins);
	for (int h = 0; h < hbins; h++) {
		for (int s = 0; s < sbins; s++) {
			float binVal = hist.at<float>(h, s);
			mVector.push_back(binVal);
		}
	}
	normalize();
}
