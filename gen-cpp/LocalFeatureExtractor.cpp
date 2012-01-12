/*
 * LocalFeatureExtractor.cpp
 *
 *  Created on: Nov 24, 2011
 *      Author: zxwang
 */

#include "LocalFeatureExtractor.h"

LocalFeatureExtractor::LocalFeatureExtractor() {
	// TODO Auto-generated constructor stub

}

LocalFeatureExtractor::~LocalFeatureExtractor() {
	// TODO Auto-generated destructor stub
}

void LocalFeatureExtractor::extractFeature(const cv::Mat& image,
		std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors) {
	try {
		cv::Ptr<cv::FeatureDetector> detector = new cv::SurfFeatureDetector();
		cv::Ptr<cv::DescriptorExtractor> extractor =
				new cv::SurfDescriptorExtractor();
		detector->detect(image, keypoints);
		extractor->compute(image, keypoints, descriptors);
	} catch (const cv::Exception& e) {
		std::cerr << e.what() << std::endl;
	}

}
