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
	try {
		/// flooding is used
//		cv::Mat imageCpy = image.clone();
//		cv::floodFill(imageCpy, cv::Point(0, 0), cv::Scalar(0), 0,
//				cv::Scalar(5, 5, 5, 0), cv::Scalar(5, 5, 5, 0), 8);
//		cv::cvtColor(imageCpy, *pMask, CV_BGR2GRAY, 1);
		/// grab cut is used
		int centerX = image.cols / 2;
		int centerY = image.rows / 2;
		float ratio = 0.9;
		int width = (int) (ratio * image.cols);
		int height = (int) (ratio * image.rows);
		cv::Rect rectangle(centerX - width / 2, centerY - height / 2, width,
				height);
		cv::Mat bgModel, fgModel;
		cv::grabCut(image, *pMask, rectangle, bgModel, fgModel, 3,
				cv::GC_INIT_WITH_RECT);
		*pMask = (*pMask) & 1;
	} catch (cv::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void Feature::compute(const cv::Mat& image){
	// clear the data vector
	mVector.clear();
	if (image.empty()) {
		return;
	}
	// resize image to have the same max dimension
	cv::Mat resizedImage;
	ImageResizer::resize(image, &resizedImage, GlobalConfig::IMAGE_LENGTH);
	// compute the segmentation, mask could be empty
	cv::Mat mask;
	segment(&mask, resizedImage);
	compute(resizedImage, mask);
}

void Feature::compute(int64_t imageKey) {
	mVector.clear();
	Image image;
	image.loadImage(imageKey);
	if (image.empty()) {
		return;
	}
	cv::Mat resizedImage;
	ImageResizer::resize(image.image, &resizedImage,
			GlobalConfig::IMAGE_LENGTH);
	Image mask;
	mask.loadMaskImage(imageKey);
	if (mask.empty()) {
		segment(&mask.image, resizedImage);
		mask.saveMaskImage(imageKey);
	}
	compute(resizedImage, mask.image);
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

void Feature::getFeature(std::vector<float>* pFeature, int index) {
	pFeature->assign(mVector.begin(), mVector.end());
}

void Feature::add(const boost::shared_ptr<ANNTreeRoot>& pRoot,
		int64_t imageId) {
	if (!empty()) {
		pRoot->addFeature(imageId, mVector);
	}
}
void Feature::knnSearch(std::vector<Neighbor>* pNeighborArray,
		boost::shared_ptr<ANNTreeRoot> pRoot, int k) {
	if (!empty()) {
		pRoot->knnSearch(pNeighborArray, mVector, k);
	}
}
