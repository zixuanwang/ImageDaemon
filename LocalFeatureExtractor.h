/*
 * LocalFeatureExtractor.h
 *
 *  Created on: Nov 24, 2011
 *      Author: zxwang
 */

#ifndef LOCALFEATUREEXTRACTOR_H_
#define LOCALFEATUREEXTRACTOR_H_
#include "GlobalConfig.h"
#include "ImageResizer.h"

class LocalFeatureExtractor {
public:
	LocalFeatureExtractor(int minFeatureNumber = 90, int maxFeatureNumber = 100,
			int maxIter = 20);
	~LocalFeatureExtractor();
	/// SURF is used
	void extractFeature(const cv::Mat& image,
			std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors);
	void resize(const cv::Mat& src, cv::Mat* pDst, int maxLength);
private:
	int mMinFeatureNumber;
	int mMaxFeatureNumber;
	int mMaxIter;
	double mThreshold;
};

#endif /* LOCALFEATUREEXTRACTOR_H_ */
