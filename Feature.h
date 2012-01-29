/*
 * Feature.h
 *
 *  Created on: Jan 21, 2012
 *      Author: zixuan
 */

#ifndef FEATURE_H_
#define FEATURE_H_
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
class Feature {
public:
	Feature();
	virtual ~Feature();
	void save(std::string* pString);
	void load(const std::string& string);
	int size();
	virtual void compute(cv::Mat& image)=0;
protected:
	std::vector<float> mVector;
};

#endif /* FEATURE_H_ */
