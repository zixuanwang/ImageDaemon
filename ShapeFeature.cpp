/*
 * ShapeFeature.cpp
 *
 *  Created on: Mar 17, 2012
 *      Author: zixuan
 */

#include "ShapeFeature.h"

ShapeFeature::ShapeFeature(int rbins, int abins) :
		Feature(), mRbins(rbins), mAbins(abins) {

}

ShapeFeature::~ShapeFeature() {

}

void ShapeFeature::load(int64_t rowKey) {
	mVector.clear();
	std::string strVector;
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	dbAdapter->loadCell(&strVector, GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_SHAPE_FEATURE_COLUMN);
	Feature::load(strVector);
}

void ShapeFeature::save(int64_t rowKey) {
	std::string strVector;
	Feature::save(&strVector);
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	dbAdapter->saveCell(strVector, GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_SHAPE_FEATURE_COLUMN);
}

void ShapeFeature::compute(const cv::Mat& image, const cv::Mat& mask) {
	if (mask.empty() || mask.type() != CV_8UC1) {
		return;
	}
	std::vector<cv::Point> foregroundPointArray;
	for (int i = 0; i < mask.rows; ++i) {
		const char* ptr = mask.ptr<char>(i);
		for (int j = 0; j < mask.cols; ++j) {
			if (ptr[j] != 0) {
				cv::Point point(j, i);
				foregroundPointArray.push_back(point);
			}
		}
	}
	if (foregroundPointArray.empty()) {
		return;
	}
	float radius = 0.0;
	cv::Point2f center;
	try {
		cv::minEnclosingCircle(cv::Mat(foregroundPointArray), center, radius);
	} catch (cv::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
	if (radius < 1.0) {
		return;
	}
	float rStep = exp(log(radius) / mRbins);
	float aStep = GlobalConfig::PI * 2.0 / mAbins;
	boost::unordered_map<int, boost::unordered_map<int, int> > histogramMap;
	for (size_t i = 0; i < foregroundPointArray.size(); ++i) {
		cv::Point& point = foregroundPointArray[i];
		float distanceY = point.y - center.y;
		float distanceX = point.x - center.x;
		float angle = 0.0;
		if (distanceY != 0.0 || distanceX != 0.0) {
			angle = atan2(distanceY, distanceX);
		}
		int aIndex = (int) ((angle + GlobalConfig::PI) / aStep);
		if (aIndex >= mAbins) {
			aIndex = mAbins - 1;
		}
		float distance = sqrt(distanceY * distanceY + distanceX * distanceX);
		int rIndex = 0;
		if (distance != 0.0) {
			rIndex = (int) (log(distance) / log(rStep));
			if (rIndex >= mRbins) {
				rIndex = mRbins - 1;
			}
		}
		histogramMap[rIndex][aIndex] += 1;
	}
	mVector.reserve(mRbins * mAbins);
	for (int i = 0; i < mRbins; ++i) {
		for (int j = 0; j < mAbins; ++j) {
			mVector.push_back(histogramMap[i][j]);
		}
	}
	normalize();
}
