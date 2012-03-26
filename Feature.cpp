/*
 * Feature.cpp
 *
 *  Created on: Jan 21, 2012
 *      Author: zixuan
 */

#include "Feature.h"

Feature::Feature() {

}

Feature::~Feature() {

}

void Feature::save(std::string* pString) {
	TypeConverter::readArrayToString(pString, mVector);
}

void Feature::load(const std::string& string) {
	TypeConverter::readStringToArray(&mVector, string);
}

int Feature::size() {
	return (int) mVector.size();
}

bool Feature::empty() {
	return mVector.empty();
}

void Feature::segment(cv::Mat* pMask, const cv::Mat& image) {
	*pMask = cv::Mat();
	if (image.empty()) {
		return;
	}
	/// flooding is used
	cv::Mat imageCpy = image.clone();
	cv::floodFill(imageCpy, cv::Point(0, 0), cv::Scalar(0), 0,
			cv::Scalar(5, 5, 5, 0), cv::Scalar(5, 5, 5, 0), 8);
	cv::cvtColor(imageCpy, *pMask, CV_BGR2GRAY, 1);
	/// grab cut is used
//	int centerX = image.cols / 2;
//	int centerY = image.rows / 2;
//	float ratio = 0.85;
//	int width = (int) (ratio * image.cols);
//	int height = (int) (ratio * image.rows);
//	cv::Rect rectangle(centerX - width / 2, centerY - height / 2, width,
//			height);
//	cv::Mat bgModel, fgModel;
//	cv::grabCut(image, *pMask, rectangle, bgModel, fgModel, 3,
//			cv::GC_INIT_WITH_RECT);
//	*pMask = (*pMask) & 1;
}

float Feature::norm() {
	double sum = 0.0;
	for (size_t i = 0; i < mVector.size(); ++i) {
		sum += mVector[i] * mVector[i];
	}
	return (float) sqrt(sum);
}

void Feature::normalize() {
	float n = norm();
	if (n != 0.0) {
		std::transform(mVector.begin(), mVector.end(), mVector.begin(),
				std::bind2nd(std::divides<float>(), n));
	}
}

const std::vector<float>& Feature::getFeature() {
	return mVector;
}
