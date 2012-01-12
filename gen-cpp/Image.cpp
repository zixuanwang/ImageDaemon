/*
 * Image.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#include "Image.h"

Image::Image() {
	// TODO Auto-generated constructor stub

}

Image::~Image() {
	// TODO Auto-generated destructor stub
}

void Image::loadImageData(int64_t rowKey){
	std::string strRowKey;
	TypeConverter::readNumToString(&strRowKey, rowKey);
	std::string strImageData;
	HbaseAdapter::instance()->loadCell(&strImageData, GlobalConfig::IMAGE_TABLE, strRowKey, GlobalConfig::IMAGE_DATA_COLUMN);
	std::vector<char> buf(strImageData.begin(), strImageData.end());
	image = cv::imdecode(cv::Mat(buf), 0);
}
