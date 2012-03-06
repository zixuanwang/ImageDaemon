/*
 * LocalFeature.cpp
 *
 *  Created on: Jan 7, 2012
 *      Author: zixuan
 */

#include "LocalFeature.h"

LocalFeature::LocalFeature() {

}

LocalFeature::~LocalFeature() {

}

bool LocalFeature::empty() const {
	return keypoint.empty();
}

void LocalFeature::load(int64_t rowKey) {
	std::string strImageFeature;
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	dbAdapter->loadCell(&strImageFeature, GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_FEATURE_COLUMN);
	load(strImageFeature);
}

void LocalFeature::save(int64_t rowKey) const {
	std::string strImageFeature;
	save(&strImageFeature);
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	dbAdapter->saveCell(strImageFeature, GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_FEATURE_COLUMN);
}

void LocalFeature::load(const std::string& string) {
	keypoint.clear();
	descriptor = cv::Mat();
	if (string.empty()) {
		return;
	}
	std::stringstream ss(
			std::stringstream::in | std::stringstream::out
					| std::stringstream::binary);
	ss.write(&string[0], string.size());
	// load keypoint
	int featureSize;
	ss.read((char*) &featureSize, sizeof(featureSize));
	keypoint.reserve(featureSize);
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
		keypoint.push_back(point);
	}
	// load descriptor
	int i, j, bytes;
	ss.read((char*) &i, sizeof(i));
	ss.read((char*) &j, sizeof(j));
	ss.read((char*) &bytes, sizeof(bytes));
	descriptor = cv::Mat(i, j, CV_32FC1);
	ss.read((char*) descriptor.data, bytes);
}

void LocalFeature::save(std::string* string) const {
	string->clear();
	if (keypoint.empty()) {
		return;
	}
	std::stringstream ss(std::stringstream::out | std::stringstream::binary);
	// save keypoint
	int featureSize = (int) keypoint.size();
	ss.write((char*) &featureSize, sizeof(featureSize));
	for (int i = 0; i < featureSize; ++i) {
		float x = keypoint[i].pt.x;
		float y = keypoint[i].pt.y;
		float size = keypoint[i].size;
		float angle = keypoint[i].angle;
		float response = keypoint[i].response;
		int octave = keypoint[i].octave;
		int class_id = keypoint[i].class_id;
		ss.write((char*) &x, sizeof(x));
		ss.write((char*) &y, sizeof(y));
		ss.write((char*) &size, sizeof(size));
		ss.write((char*) &angle, sizeof(angle));
		ss.write((char*) &response, sizeof(response));
		ss.write((char*) &octave, sizeof(octave));
		ss.write((char*) &class_id, sizeof(class_id));
	}
	// save descriptor
	int i = descriptor.rows;
	int j = descriptor.cols;
	int bytes = (int) descriptor.step * i;
	ss.write((char*) &i, sizeof(i));
	ss.write((char*) &j, sizeof(j));
	ss.write((char*) &bytes, sizeof(bytes));
	ss.write((char*) descriptor.data, bytes);
	*string = ss.str();
}
