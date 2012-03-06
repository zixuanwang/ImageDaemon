/*
 * ImageRecognizer.cpp
 *
 *  Created on: Feb 18, 2012
 *      Author: zixuan
 */

#include "ImageRecognizer.h"

ImageRecognizer::ImageRecognizer() {

}

ImageRecognizer::~ImageRecognizer() {

}

void ImageRecognizer::queryBoWFeature(std::vector<std::string>* pReturnArray,
		const std::string& imagePath) {
	pReturnArray->clear();
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	cv::Mat image = cv::imread(imagePath, 0);
	LocalFeature localFeature;
	LocalFeatureExtractor localFeatureExtractor;
	localFeatureExtractor.extractFeature(image, localFeature.keypoint,
			localFeature.descriptor);
	BoWHistogram histogram;
	ANNVocabulary::instance()->quantizeFeature(localFeature, &histogram);
	const std::vector<int64_t>& visualwordIdArray =
			histogram.visualwordIdArray();
	const std::vector<double>& frequencyArray = histogram.frequencyArray();
	std::cout << "histogram size: " << histogram.size() << std::endl;
	// TODO: modify the query pipeline
	boost::unordered_map<int64_t, double> candidateScoreMap;
	Ticker ticker;
	ticker.start();
	for (size_t i = 0; i < visualwordIdArray.size(); ++i) {
		std::vector<Posting> postingArray;
		InvertedIndexClient invertedIndexClient;
		invertedIndexClient.loadPostingList(&postingArray,
				visualwordIdArray[i]);
		for (size_t j = 0; j < postingArray.size(); ++j) {
			candidateScoreMap[postingArray[j].imageId] += postingArray[j].score
					* frequencyArray[i];
		}
	}
	ticker.stop();
	ticker.start();
	std::vector<RankItem<int64_t, double> > candidateRankList;
	candidateRankList.reserve(candidateScoreMap.size());
	for (boost::unordered_map<int64_t, double>::iterator iter =
			candidateScoreMap.begin(); iter != candidateScoreMap.end();
			++iter) {
		RankItem<int64_t, double> item(iter->first, iter->second);
		candidateRankList.push_back(item);
	}
	size_t candidateSize = (size_t) GlobalConfig::CANDIDATE_COUNT;
	if (candidateRankList.size() > candidateSize) {
		std::nth_element(candidateRankList.begin(),
				candidateRankList.begin() + candidateSize,
				candidateRankList.end());
	}
	ticker.stop();
	ticker.start();
	std::vector<double> queryVector;
	histogram.flatten(&queryVector, ANNVocabulary::instance()->size());
	std::vector<RankItem<int64_t, double> > rankList;
	for (size_t i = 0; i < candidateRankList.size() && i < candidateSize; ++i) {
		int64_t otherImageId = candidateRankList[i].index;
		BoWHistogram otherHistogram;
		otherHistogram.load(otherImageId);
		double score = otherHistogram.innerProduct(queryVector);
		RankItem<int64_t, double> item(otherImageId, -1.0 * score);
		rankList.push_back(item);
	}
	std::sort(rankList.begin(), rankList.end());
	ticker.stop();
	for (int i = 0;
			i < GlobalConfig::VERIFICATION_COUNT && i < (int) rankList.size();
			++i) {
		RobustMatcher rmatcher;
		LocalFeature otherLocalFeature;
		otherLocalFeature.load(rankList[i].index);
		std::vector<cv::DMatch> matches;
		cv::Mat homography = rmatcher.match(localFeature, otherLocalFeature,
				&matches);
		if (matches.size() >= 7) {
			std::string path;
			dbAdapter->loadCell(&path, GlobalConfig::IMAGE_TABLE,
					rankList[i].index, GlobalConfig::IMAGE_HASH_COLUMN);
			pReturnArray->push_back(path);
		}
	}
}
