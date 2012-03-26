/*
 * SURFFeature.cpp
 *
 *  Created on: Mar 24, 2012
 *      Author: zixuan
 */

#include "SURFFeature.h"

SURFFeature::SURFFeature(int featureCount, int maxIter) :
		mMaxIter(maxIter) {
	mMinFeatureCount = (int) ((float) featureCount * 0.95);
	mMaxFeatureCount = (int) ((float) featureCount / 0.95);
	mThreshold = 800;
}

SURFFeature::~SURFFeature() {

}

void SURFFeature::load(int64_t rowKey) {
	mVector.clear();
	std::string strVector;
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	dbAdapter->loadCell(&strVector, GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_SURF_FEATURE_COLUMN);
	Feature::load(strVector);
	mFeatureCount = (int) mVector.size() / GlobalConfig::SURF_FEATURE_DIMENSION;
}

void SURFFeature::save(int64_t rowKey) {
	std::string strVector;
	Feature::save(&strVector);
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	dbAdapter->saveCell(strVector, GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_SURF_FEATURE_COLUMN);
}

void SURFFeature::compute(const cv::Mat& image) {
	mVector.clear();
	if (image.empty()) {
		return;
	}
	try {
		cv::Mat grayImage;
		cv::cvtColor(image, grayImage, CV_BGR2GRAY);
		cv::Mat resizedImage;
		ImageResizer::resize(grayImage, &resizedImage,
				GlobalConfig::IMAGE_LENGTH);
		std::vector<cv::KeyPoint> keypoints;
		for (int i = 0; i < mMaxIter; ++i) {
			cv::Ptr<cv::FeatureDetector> detector = new cv::SurfFeatureDetector(
					mThreshold);
			keypoints.clear();
			detector->detect(resizedImage, keypoints);
			int size = (int) keypoints.size();
			if (size >= mMinFeatureCount && size <= mMaxFeatureCount) {
				break;
			}
			if (size < mMinFeatureCount) {
				mThreshold *= 0.85;
				if (mThreshold < 1.15) {
					mThreshold = 1.15;
				}
			}
			if (size > mMaxFeatureCount) {
				mThreshold *= 1.15;
			}
		}
		// compute the feature descriptor
		cv::Ptr<cv::DescriptorExtractor> extractor =
				new cv::SurfDescriptorExtractor();
		cv::Mat descriptors;
		extractor->compute(resizedImage, keypoints, descriptors);
		// get the number of features in this image
		mFeatureCount = descriptors.rows;
		// assign the data
		float* ptr = (float*) descriptors.data;
		mVector.assign(ptr, ptr + descriptors.rows * descriptors.cols);
	} catch (const cv::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
	//TODO: other normalization
//	normalize();
}
