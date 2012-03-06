/*
 * Image.h
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "GlobalConfig.h"
#include "HbaseAdapter.h"
class Image {
public:
	Image();
	~Image();
	cv::Mat image;
	void load(int64_t rowKey);
	void load(const std::string& imageHash);
};

#endif /* IMAGE_H_ */
