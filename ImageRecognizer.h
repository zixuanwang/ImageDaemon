/*
 * ImageRecognizer.h
 *
 *  Created on: Feb 18, 2012
 *      Author: zixuan
 */

#ifndef IMAGERECOGNIZER_H_
#define IMAGERECOGNIZER_H_
#include "GlobalConfig.h"
#include "RobustMatcher.h"
#include "HbaseAdapter.h"
#include "BoWHistogram.h"
#include "LocalFeatureExtractor.h"
#include "LocalFeature.h"
#include "RankItem.h"
#include "Ticker.h"
#include "ANNVocabulary.h"
#include "InvertedIndexClient.h"
class ImageRecognizer {
public:
	ImageRecognizer();
	virtual ~ImageRecognizer();
	void queryBoWFeature(std::vector<std::string>* pReturnArray,
			const std::string& imagePath);

};

#endif /* IMAGERECOGNIZER_H_ */
