/*
 * test.h
 *
 *  Created on: Jan 11, 2012
 *      Author: zixuan
 */

#ifndef TEST_H_
#define TEST_H_
#include <boost/unordered_set.hpp>
#include <algorithm>
#include <numeric>
#include "Image.h"
#include "LocalFeature.h"
#include "LocalFeatureExtractor.h"
#include "ImageDaemon_types.h"
#include "RankItem.h"
using namespace ::ImageDaemon;
void testFeature() {
	int64_t rowKey = 0;
	Image image;
	image.loadImageData(rowKey);
	LocalFeatureExtractor localFeatureExtractor;
	LocalFeature localFeature;
	localFeatureExtractor.extractFeature(image.image, localFeature.keypoint,
			localFeature.descriptor);
	localFeature.save(rowKey);
	LocalFeature localFeature2;
	localFeature2.load(rowKey);
	for (size_t i = 0; i < localFeature2.keypoint.size(); ++i) {
		cv::circle(image.image, localFeature2.keypoint[i].pt,
				localFeature2.keypoint[i].size, CV_RGB(255,0,0), 1, CV_AA, 0);
	}
	cv::namedWindow("test");
	cv::imshow("test", image.image);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void testHistogram() {
	int64_t rowKey = 0;
	Image image;
	image.loadImageData(rowKey);
	LocalFeature localFeature;
	LocalFeatureExtractor localFeatureExtractor;
	localFeatureExtractor.extractFeature(image.image, localFeature.keypoint,
			localFeature.descriptor);
	localFeature.save(rowKey);
	BoWHistogram histogram;
	ANNVocabulary::instance()->quantizeFeature(localFeature, &histogram);
	std::vector<Bin> _return;
	histogram.convert(&_return);
	for (size_t i = 0; i < _return.size(); ++i) {
		std::cout << _return[i].visualwordID << "\t" << _return[i].frequency
				<< std::endl;
	}
}

void testQuery() {
	std::string testImagePath = "/home/zixuan/Desktop/1.jpg";
	cv::Mat image = cv::imread(testImagePath, 0);
	LocalFeature localFeature;
	LocalFeatureExtractor localFeatureExtractor;
	localFeatureExtractor.extractFeature(image, localFeature.keypoint,
			localFeature.descriptor);
	BoWHistogram histogram;
	ANNVocabulary::instance()->quantizeFeature(localFeature, &histogram);
	const std::vector<int64_t>& visualwordIdArray =
			histogram.visualwordIdArray();
	std::cout << "histogram size: " << visualwordIdArray.size() << std::endl;
//	for (int i = 0; i < visualwordIdArray.size(); ++i) {
//		std::cout << visualwordIdArray[i] << "\t";
//	}
	// TODO: add authority score to ranking.
	boost::unordered_set<int64_t> candidateIdSet;
	for (size_t i = 0; i < visualwordIdArray.size(); ++i) {
		std::vector<int64_t> imageIdArray;
		InvertedIndex::instance()->loadPostingList(&imageIdArray,
				visualwordIdArray[i]);
		candidateIdSet.insert(imageIdArray.begin(), imageIdArray.end());
	}
	std::cout << "candidate set size: " << candidateIdSet.size() << std::endl;
	for (boost::unordered_set<int64_t>::iterator iter = candidateIdSet.begin();
			iter != candidateIdSet.end(); ++iter) {
		std::cout << *iter << "\t";
	}
	std::cout << std::endl;
	std::vector<double> queryVector;
	histogram.flatten(&queryVector, ANNVocabulary::instance()->size());
	std::vector<RankItem<int64_t, double> > rankList;
	for (boost::unordered_set<int64_t>::iterator iter = candidateIdSet.begin();
			iter != candidateIdSet.end(); ++iter) {
		int64_t otherImageId = *iter;
		BoWHistogram otherHistogram;
		otherHistogram.load(otherImageId);
		double score = otherHistogram.innerProduct(queryVector);
		RankItem<int64_t, double> item(otherImageId, -1.0 * score);
		rankList.push_back(item);
		//otherHistogram.print();
		std::cout << score << std::endl;
	}
	if (rankList.size() < 10 + 1) {
		std::nth_element(rankList.begin(), rankList.end(), rankList.end());
	} else {
		std::nth_element(rankList.begin(), rankList.begin() + 10 + 1,
				rankList.end());
	}
	cv::namedWindow("test");
	for (int i = 0; i < 10 && i < (int) rankList.size(); ++i) {
		Image result;
		result.loadImageData(rankList[i].index);
		std::cout << rankList[i].value << std::endl;
		cv::imshow("test", result.image);
		cv::waitKey(0);
	}
}

#endif /* TEST_H_ */
