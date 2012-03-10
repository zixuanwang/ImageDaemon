/*
 * ImageResizer.h
 *
 *  Created on: Mar 10, 2012
 *      Author: zixuan
 */

#ifndef IMAGERESIZER_H_
#define IMAGERESIZER_H_

#include "GlobalConfig.h"
class ImageResizer {
public:
	ImageResizer();
	~ImageResizer();
	static void crop(const std::string& imagePath,
			const std::string& cropImagePath, int width, int height);
	static void resize(const cv::Mat& src, cv::Mat* pDst, int maxLength);
};

#endif /* IMAGERESIZER_H_ */
