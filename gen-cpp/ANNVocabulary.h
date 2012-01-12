/*
 * ANNVocabulary.h
 *
 *  Created on: May 12, 2011
 *      Author: zxwang
 */

#ifndef ANNVOCABULARY_H_
#define ANNVOCABULARY_H_
#include <fstream>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include "LocalFeature.h"
#include "BoWHistogram.h"

/// Singleton class
class ANNVocabulary {
public:
	static ANNVocabulary* instance();
	void init(const std::string& vocabularyPath);
	/// lnc is used.
	void quantizeFeature(const LocalFeature& localFeature,BoWHistogram* pHistogram);
	/// Return vocabulary size.
	int64_t size() {
		return (int64_t) mVocabularyMat.rows;
	}
private:
	cv::Mat mVocabularyMat;
	boost::shared_ptr<cv::flann::Index> mpVocabularyIndex;
	const static int SOFT_ASSIGN;
	static ANNVocabulary* pInstance;
	// Default constuctor, copy constructor and assignment operators should be as private so that nobody
	// from outside can call those functions and instantiate it
	ANNVocabulary();
	ANNVocabulary(const ANNVocabulary&);
	ANNVocabulary& operator=(const ANNVocabulary&);
};

#endif /* ANNVOCABULARY_H_ */
