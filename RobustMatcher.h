/*
 * RobustMatcher.h
 *
 *  Created on: Feb 18, 2012
 *      Author: zixuan
 */

#ifndef ROBUSTMATCHER_H_
#define ROBUSTMATCHER_H_
#include "GlobalConfig.h"
#include "LocalFeature.h"
class RobustMatcher {
private:
	// pointer to the feature point detector object
	cv::Ptr<cv::FeatureDetector> detector;
	// pointer to the feature descriptor extractor object
	cv::Ptr<cv::DescriptorExtractor> extractor;
	float ratio; // max ratio between 1st and 2nd NN
	double distance; // min distance
public:
	RobustMatcher();
	~RobustMatcher();
	cv::Mat match(const LocalFeature& localFeature1,
			const LocalFeature& localFeature2,
			std::vector<cv::DMatch>* pMatches);
	void setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect);
	void setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc);
	int ratioTest(std::vector<std::vector<cv::DMatch> > &matches);
	// Insert symmetrical matches in symMatches vector
	void symmetryTest(const std::vector<std::vector<cv::DMatch> >& matches1,
			const std::vector<std::vector<cv::DMatch> >& matches2,
			std::vector<cv::DMatch>& symMatches);
	cv::Mat ransacTest(const std::vector<cv::DMatch>& matches,
			const std::vector<cv::KeyPoint>& keypoints1,
			const std::vector<cv::KeyPoint>& keypoints2,
			std::vector<cv::DMatch>& outMatches);
};

#endif /* ROBUSTMATCHER_H_ */
