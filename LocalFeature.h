/*
 * LocalFeature.h
 *
 *  Created on: Jan 7, 2012
 *      Author: zixuan
 */

#ifndef LOCALFEATURE_H_
#define LOCALFEATURE_H_

#include "GlobalConfig.h"
#include "HbaseAdapter.h"
class LocalFeature {
public:
	LocalFeature();
	~LocalFeature();
	bool empty() const;
	void load(int64_t rowKey);
	void save(int64_t rowKey) const;
	void load(const std::string& string);
	void save(std::string* string) const;
	std::vector<cv::KeyPoint> keypoint;
	cv::Mat descriptor;
private:

};

#endif /* LOCALFEATURE_H_ */
