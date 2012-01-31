/*
 * BoWHistogram.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#include "BoWHistogram.h"

BoWHistogram::BoWHistogram() {
	// TODO Auto-generated constructor stub

}

BoWHistogram::~BoWHistogram() {
	// TODO Auto-generated destructor stub
}

bool BoWHistogram::empty() const {
	return mHistogram.first.empty();
}

void BoWHistogram::load(int64_t rowKey) {
	clear();
	std::string strHistogram;
	MongoDBAdapter::instance()->loadCell(&strHistogram,
			GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_BOW_HISTOGRAM_COLUMN);
	if (strHistogram.empty()) {
		return;
	}
	int histogramSize = (int) strHistogram.size()
			/ (sizeof(int64_t) + sizeof(double));
	mHistogram.first.assign(histogramSize, 0);
	mHistogram.second.assign(histogramSize, 0);
	memcpy((char*) &mHistogram.first[0], (char*) &strHistogram[0],
			histogramSize * sizeof(int64_t));
	memcpy((char*) &mHistogram.second[0],
			(char*) &strHistogram[strHistogram.size() / 2],
			histogramSize * sizeof(double));
}

void BoWHistogram::save(int64_t rowKey) const {
	if (empty()) {
		return;
	}
	std::stringstream ss(std::stringstream::out | std::stringstream::binary);
	int64_t histogramSize = size();
	for (int i = 0; i < histogramSize; ++i) {
		int64_t visualwordID = mHistogram.first[i];
		ss.write((char*) &visualwordID, sizeof(visualwordID));
	}
	for (int i = 0; i < histogramSize; ++i) {
		double frequency = mHistogram.second[i];
		ss.write((char*) &frequency, sizeof(frequency));
	}
	std::string strHistogram = ss.str();
	MongoDBAdapter::instance()->saveCell(strHistogram,
			GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_BOW_HISTOGRAM_COLUMN);
}

void BoWHistogram::flatten(std::vector<double>* pVector,
		int64_t vocabularySize) {
	pVector->assign(vocabularySize, 0);
	int64_t histogramSize = size();
	for (int64_t i = 0; i < histogramSize; ++i) {
		int64_t visualwordID = mHistogram.first[i];
		double frequency = mHistogram.second[i];
		(*pVector)[visualwordID] = frequency;
	}
}

void BoWHistogram::clear() {
	mHistogram.first.clear();
	mHistogram.second.clear();
}

void BoWHistogram::normalize() {
	double norm = 0.0;
	int64_t histogramSize = size();

	for (int64_t i = 0; i < histogramSize; ++i) {
		norm += mHistogram.second[i] * mHistogram.second[i];
	}
	norm = sqrt(norm);
	for (int64_t i = 0; i < histogramSize; ++i) {
		mHistogram.second[i] /= norm;
	}
}

int64_t BoWHistogram::size() const {
	return (int64_t) mHistogram.first.size();
}

void BoWHistogram::addBin(int64_t visualwordID, double frequency) {
	mHistogram.first.push_back(visualwordID);
	mHistogram.second.push_back(frequency);
}

void BoWHistogram::convert(std::vector<Bin>* pVector) {
	int64_t histogramSize = size();
	pVector->assign(histogramSize, Bin());
	for (int64_t i = 0; i < histogramSize; ++i) {
		(*pVector)[i].visualwordID = mHistogram.first[i];
		(*pVector)[i].frequency = mHistogram.second[i];
	}
}

const std::vector<int64_t>& BoWHistogram::visualwordIdArray() {
	return mHistogram.first;
}

const std::vector<double>& BoWHistogram::frequencyArray() {
	return mHistogram.second;
}

double BoWHistogram::innerProduct(const std::vector<double>& vector) {
	double score = 0.0;
	int64_t histogramSize = size();
	for (int64_t i = 0; i < histogramSize; ++i) {
		int64_t visualwordID = mHistogram.first[i];
		double frequency = mHistogram.second[i];
		score += vector[visualwordID] * frequency;
	}
	return score;
}
void BoWHistogram::print() const {
	int64_t histogramSize = size();
	for (int64_t i = 0; i < histogramSize; ++i) {
		std::cout << mHistogram.first[i] << "\t" << mHistogram.second[i]
				<< std::endl;
	}
}
