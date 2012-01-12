/*
 * BoWHistogram.h
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#ifndef BOWHISTOGRAM_H_
#define BOWHISTOGRAM_H_

#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include "GlobalConfig.h"
#include "HbaseAdapter.h"
#include "ImageDaemon_types.h"
using namespace ::ImageDaemon;
class BoWHistogram {
public:
	BoWHistogram();
	~BoWHistogram();
	void load(int64_t rowKey);
	void save(int64_t rowKey) const;
	void flatten(std::vector<double>* pVector, int64_t vocabularySize);
	void convert(std::vector<Bin>* pVector);
	void clear();
	void addBin(int64_t visualwordID, double frequency);
	void normalize();
	const std::vector<int64_t>& visualwordIdArray();
	double innerProduct(const std::vector<double>& vector);
	int64_t size() const;
	void print() const;
private:
	std::pair<std::vector<int64_t>, std::vector<double> > mHistogram;
//	std::vector<std::pair<int64_t, double> > mHistogram;
};

#endif /* BOWHISTOGRAM_H_ */
