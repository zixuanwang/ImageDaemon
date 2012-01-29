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
#include "MongoDBAdapter.h"
class Image {
public:
	Image();
	~Image();
	int64_t id();
	cv::Mat image;
	void load(int64_t rowKey);
private:
	int64_t mId;
	std::string mImageHash;
//	void uniqueId();
//	void loadFromHbase(const std::string& imageHash);
};

#endif /* IMAGE_H_ */
