/*
 * LocalFeatureExtractor.h
 *
 *  Created on: Nov 24, 2011
 *      Author: zxwang
 */

#ifndef LOCALFEATUREEXTRACTOR_H_
#define LOCALFEATUREEXTRACTOR_H_
#include <opencv2/opencv.hpp>
#include <fstream>
#include "TypeConverter.h"

class LocalFeatureExtractor {
public:
	LocalFeatureExtractor();
	~LocalFeatureExtractor();
	/// SURF is used
	void extractFeature(const cv::Mat& image,
			std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors);
};

#endif /* LOCALFEATUREEXTRACTOR_H_ */
