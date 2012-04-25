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
	mThreshold = 1500;
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
	// save descriptor
	std::string strVector;
	Feature::save(&strVector);
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	dbAdapter->saveCell(strVector, GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_SURF_FEATURE_COLUMN);
	// save key points
	std::string strKeypoint;
	saveKeypoint(&strKeypoint);
	dbAdapter->saveCell(strKeypoint, GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_SURF_KEYPOINT_COLUMN);
}

void SURFFeature::compute(const cv::Mat& image, const cv::Mat& mask) {
	try {
		cv::Mat grayImage;
		cv::cvtColor(image, grayImage, CV_BGR2GRAY);
		std::vector<cv::KeyPoint> keypoints;
		for (int i = 0; i < mMaxIter; ++i) {
			cv::Ptr<cv::FeatureDetector> detector = new cv::SurfFeatureDetector(
					mThreshold);
			keypoints.clear();
			detector->detect(grayImage, keypoints, mask);
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
		// if too many keypoints, only keep first mMaxFeatureCount
		if ((int) keypoints.size() > mMaxFeatureCount) {
			keypoints.erase(keypoints.begin() + mMaxFeatureCount,
					keypoints.end());
		}
		cv::Ptr<cv::DescriptorExtractor> extractor =
				new cv::SurfDescriptorExtractor();
		cv::Mat descriptors;
		extractor->compute(grayImage, keypoints, descriptors);
		// get the number of features in this image
		mFeatureCount = descriptors.rows;
		// assign the data
		float* ptr = (float*) descriptors.data;
		mVector.assign(ptr, ptr + descriptors.rows * descriptors.cols);
		// save key points
		mKeypoint.assign(keypoints.begin(), keypoints.end());
	} catch (const cv::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void SURFFeature::add(const boost::shared_ptr<ANNTreeRoot>& pRoot,
		int64_t imageId) {
	for (int i = 0; i < mFeatureCount; ++i) {
		std::vector<float> feature;
		getFeature(&feature, i);
		pRoot->addFeature(imageId, feature);
	}
}
void SURFFeature::knnSearch(std::vector<Neighbor>* pNeighborArray,
		boost::shared_ptr<ANNTreeRoot> pRoot, int k) {
	pNeighborArray->clear();
	std::vector<std::vector<float> > featureArray;
	featureArray.reserve(mFeatureCount);
	for (int i = 0; i < mFeatureCount; ++i) {
		std::vector<float> feature;
		getFeature(&feature, i);
		featureArray.push_back(feature);
	}
	pRoot->knnSearch(pNeighborArray, featureArray, k);
}

void SURFFeature::getFeature(std::vector<float>* pFeature, int index) {
	pFeature->clear();
	if (index >= 0 && index < mFeatureCount) {
		pFeature->assign(
				mVector.begin() + index * GlobalConfig::SURF_FEATURE_DIMENSION,
				mVector.begin()
						+ (index + 1) * GlobalConfig::SURF_FEATURE_DIMENSION);
	}
}

void SURFFeature::saveKeypoint(std::string* pString) {
	pString->clear();
	if (mKeypoint.empty()) {
		return;
	}
	std::stringstream ss(std::stringstream::out | std::stringstream::binary);
	int featureSize = (int) mKeypoint.size();
	ss.write((char*) &featureSize, sizeof(featureSize));
	for (int i = 0; i < featureSize; ++i) {
		cv::KeyPoint& keypoint = mKeypoint[i];
		float x = keypoint.pt.x;
		float y = keypoint.pt.y;
		float size = keypoint.size;
		float angle = keypoint.angle;
		float response = keypoint.response;
		int octave = keypoint.octave;
		int class_id = keypoint.class_id;
		ss.write((char*) &x, sizeof(x));
		ss.write((char*) &y, sizeof(y));
		ss.write((char*) &size, sizeof(size));
		ss.write((char*) &angle, sizeof(angle));
		ss.write((char*) &response, sizeof(response));
		ss.write((char*) &octave, sizeof(octave));
		ss.write((char*) &class_id, sizeof(class_id));
	}
	*pString = ss.str();
}

void SURFFeature::loadKeypoint(const std::string& string) {
	mKeypoint.clear();
	if (string.empty()) {
		return;
	}
	std::stringstream ss(
			std::stringstream::in | std::stringstream::out
					| std::stringstream::binary);
	// write data to ss
	ss.write(&string[0], string.size());
	// load keypoint
	int featureSize;
	ss.read((char*) &featureSize, sizeof(featureSize));
	mKeypoint.reserve(featureSize);
	for (int i = 0; i < featureSize; ++i) {
		float x, y, size, angle, response;
		int octave, class_id;
		ss.read((char*) &x, sizeof(x));
		ss.read((char*) &y, sizeof(y));
		ss.read((char*) &size, sizeof(size));
		ss.read((char*) &angle, sizeof(angle));
		ss.read((char*) &response, sizeof(response));
		ss.read((char*) &octave, sizeof(octave));
		ss.read((char*) &class_id, sizeof(class_id));
		cv::KeyPoint point;
		point.pt = cv::Point2f(x, y);
		point.size = size;
		point.angle = angle;
		point.response = response;
		point.octave = octave;
		point.class_id = class_id;
		mKeypoint.push_back(point);
	}
}
