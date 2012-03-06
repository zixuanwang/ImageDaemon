/*
 * RobustMatcher.cpp
 *
 *  Created on: Feb 18, 2012
 *      Author: zixuan
 */

#include "RobustMatcher.h"

RobustMatcher::RobustMatcher() :
		ratio(0.75), distance(1.0) {
	// SURF is the default feature
	detector = new cv::SurfFeatureDetector();
	extractor = new cv::SurfDescriptorExtractor();
}

RobustMatcher::~RobustMatcher() {
	// TODO Auto-generated destructor stub
}

cv::Mat RobustMatcher::match(const LocalFeature& localFeature1,
		const LocalFeature& localFeature2, std::vector<cv::DMatch>* pMatches) {
	cv::BruteForceMatcher<cv::L2<float> > matcher;
	// from image 1 to image 2
	// based on k nearest neighbours (with k=2)
	std::vector<std::vector<cv::DMatch> > matches1;
	matcher.knnMatch(localFeature1.descriptor, localFeature2.descriptor,
			matches1, // vector of matches (up to 2 per entry)
			2);
	// return 2 nearest neighbours
	// from image 2 to image 1
	// based on k nearest neighbours (with k=2)
	std::vector<std::vector<cv::DMatch> > matches2;
	matcher.knnMatch(localFeature2.descriptor, localFeature1.descriptor,
			matches2, // vector of matches (up to 2 per entry)
			2);
	// return 2 nearest neighbours
	// 3. Remove matches for which NN ratio is
	// > than threshold
	// clean image 1 -> image 2 matches
	ratioTest(matches1);
	// clean image 2 -> image 1 matches
	ratioTest(matches2);
	// 4. Remove non-symmetrical matches
	std::vector<cv::DMatch> symMatches;
	symmetryTest(matches1, matches2, symMatches);
	if (symMatches.size() < 4) {
		return cv::Mat();
	}
	// 5. Validate matches using RANSAC
	cv::Mat homography = ransacTest(symMatches, localFeature1.keypoint,
			localFeature2.keypoint, *pMatches);
	// return the found homography matrix
	return homography;
}

// Clear matches for which NN ratio is > than threshold
// return the number of removed points
// (corresponding entries being cleared,
// i.e. size will be 0)
int RobustMatcher::ratioTest(std::vector<std::vector<cv::DMatch> > &matches) {
	int removed = 0;
	// for all matches
	for (std::vector<std::vector<cv::DMatch> >::iterator matchIterator =
			matches.begin(); matchIterator != matches.end(); ++matchIterator) {
		// if 2 NN has been identified
		if (matchIterator->size() > 1) {
			// check distance ratio
			if ((*matchIterator)[0].distance / (*matchIterator)[1].distance
					> ratio) {
				matchIterator->clear(); // remove match
				removed++;
			}
		} else { // does not have 2 neighbours
			matchIterator->clear(); // remove match
			removed++;
		}
	}
	return removed;
}
// Insert symmetrical matches in symMatches vector
void RobustMatcher::symmetryTest(
		const std::vector<std::vector<cv::DMatch> >& matches1,
		const std::vector<std::vector<cv::DMatch> >& matches2,
		std::vector<cv::DMatch>& symMatches) {
	// for all matches image 1 -> image 2
	for (std::vector<std::vector<cv::DMatch> >::const_iterator matchIterator1 =
			matches1.begin(); matchIterator1 != matches1.end();
			++matchIterator1) {
		// ignore deleted matches
		if (matchIterator1->size() < 2)
			continue;
		// for all matches image 2 -> image 1
		for (std::vector<std::vector<cv::DMatch> >::const_iterator matchIterator2 =
				matches2.begin(); matchIterator2 != matches2.end();
				++matchIterator2) {
			// ignore deleted matches
			if (matchIterator2->size() < 2)
				continue;
			// Match symmetry test
			if ((*matchIterator1)[0].queryIdx == (*matchIterator2)[0].trainIdx
					&& (*matchIterator2)[0].queryIdx
							== (*matchIterator1)[0].trainIdx) {
				// add symmetrical match
				symMatches.push_back(
						cv::DMatch((*matchIterator1)[0].queryIdx,
								(*matchIterator1)[0].trainIdx,
								(*matchIterator1)[0].distance));
				break; // next match in image 1 -> image 2
			}
		}
	}
}

// Identify good matches using RANSAC
// Return fundemental matrix
cv::Mat RobustMatcher::ransacTest(const std::vector<cv::DMatch>& matches,
		const std::vector<cv::KeyPoint>& keypoints1,
		const std::vector<cv::KeyPoint>& keypoints2,
		std::vector<cv::DMatch>& outMatches) {
	// Convert keypoints into Point2f
	std::vector<cv::Point2f> points1, points2;
	for (std::vector<cv::DMatch>::const_iterator it = matches.begin();
			it != matches.end(); ++it) {
		// Get the position of left keypoints
		float x = keypoints1[it->queryIdx].pt.x;
		float y = keypoints1[it->queryIdx].pt.y;
		points1.push_back(cv::Point2f(x, y));
		// Get the position of right keypoints
		x = keypoints2[it->trainIdx].pt.x;
		y = keypoints2[it->trainIdx].pt.y;
		points2.push_back(cv::Point2f(x, y));
	}
	std::vector<uchar> inliers(points1.size(), 0);
	cv::Mat homography = cv::findHomography(cv::Mat(points1), cv::Mat(points2),
			inliers, CV_RANSAC, distance);
	// extract the surviving (inliers) matches
	std::vector<uchar>::const_iterator itIn = inliers.begin();
	std::vector<cv::DMatch>::const_iterator itM = matches.begin();
	// for all matches
	for (; itIn != inliers.end(); ++itIn, ++itM) {
		if (*itIn) { // it is a valid match
			outMatches.push_back(*itM);
		}
	}
	return homography;
}

// Set the feature detector
void RobustMatcher::setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect) {
	detector = detect;
}
// Set the descriptor extractor
void RobustMatcher::setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc) {
	extractor = desc;
}
