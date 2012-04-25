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
	// image raw data are stored in hbase
	void load(int64_t rowKey, const std::string& columnName);
	void save(int64_t rowKey, const std::string& columnName);
	// save image in filesystem
	void load(const std::string& imagePath, int flag = 1);
	void save(const std::string& imagePath);
	// helper functions
	void loadImage(int64_t imageKey);
	void loadCropImage(int64_t imageKey);
	void saveCropImage(int64_t imageKey);
	void loadMaskImage(int64_t imageKey);
	void saveMaskImage(int64_t imageKey);
	static void getImageHash(std::string* pImageHash, int64_t imageKey);

	bool empty();

	// deprecated functions
	void load(int64_t rowKey);
};

#endif /* IMAGE_H_ */
