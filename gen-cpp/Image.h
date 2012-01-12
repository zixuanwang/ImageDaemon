/*
 * Image.h
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <opencv2/opencv.hpp>
#include "GlobalConfig.h"
#include "HbaseAdapter.h"
class Image {
public:
	Image();
	~Image();
	void loadImageData(int64_t rowKey);
	cv::Mat image;
};

#endif /* IMAGE_H_ */
