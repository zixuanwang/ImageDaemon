/*
 * LocalFeatureExtractor.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: zxwang
 */

#include "LocalFeatureExtractor.h"

LocalFeatureExtractor::LocalFeatureExtractor(int minFeatureNumber,
		int maxFeatureNumber, int maxIter) :
		mMinFeatureNumber(minFeatureNumber), mMaxFeatureNumber(
				maxFeatureNumber), mMaxIter(maxIter), mThreshold(500.0) {

}

LocalFeatureExtractor::~LocalFeatureExtractor() {
}

void LocalFeatureExtractor::extractFeature(const cv::Mat& image,
		std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors) {
	try {
		keypoints.clear();
		descriptors = cv::Mat();
		if (image.data == NULL) {
			return;
		}
		cv::Mat resizedImage;
		ImageResizer::resize(image, &resizedImage, GlobalConfig::IMAGE_LENGTH);
		if (resizedImage.data == NULL) {
			return;
		}
		for (int i = 0; i < mMaxIter; ++i) {
			cv::Ptr<cv::FeatureDetector> detector = new cv::SurfFeatureDetector(
					mThreshold);
			keypoints.clear();
			detector->detect(resizedImage, keypoints);
			int size = (int) keypoints.size();
			if (size >= mMinFeatureNumber && size <= mMaxFeatureNumber) {
				break;
			}
			if (size < mMinFeatureNumber) {
				mThreshold *= 0.85;
				if (mThreshold < 1.15) {
					mThreshold = 1.15;
				}
			}
			if (size > mMaxFeatureNumber) {
				mThreshold *= 1.15;
			}
		}
		cv::Ptr<cv::DescriptorExtractor> extractor =
				new cv::SurfDescriptorExtractor();
		extractor->compute(resizedImage, keypoints, descriptors);
	} catch (const cv::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void LocalFeatureExtractor::resize(const cv::Mat& src, cv::Mat* pDst,
		int maxLength) {
	int rows = src.rows;
	int cols = src.cols;
	*pDst = cv::Mat();
	if (rows == 0 || cols == 0) {
		return;
	}
	int length = rows > cols ? rows : cols;
	double ratio = (double) maxLength / length;
	cv::resize(src, *pDst, cv::Size(), ratio, ratio, cv::INTER_LINEAR);
}
