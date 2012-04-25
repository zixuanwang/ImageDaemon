/*
 * Image.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#include "Image.h"

Image::Image() {
}

Image::~Image() {

}

void Image::load(int64_t rowKey, const std::string& columnName) {
	image = cv::Mat();
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	std::string strImageData;
	dbAdapter->loadCell(&strImageData, GlobalConfig::IMAGE_TABLE, rowKey,
			columnName);
	if (!strImageData.empty()) {
		std::vector<char> buf(strImageData.begin(), strImageData.end());
		image = cv::imdecode(cv::Mat(buf), 1);
	}
}

void Image::save(int64_t rowKey, const std::string& columnName) {
	if (!empty()) {
		std::vector<uchar> buf;
		cv::imencode(".jpg", image, buf);
		std::string strImageData(buf.begin(), buf.end());
		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
		dbAdapter->saveCell(strImageData, GlobalConfig::IMAGE_TABLE, rowKey,
				columnName);
	}
}

void Image::load(const std::string& imagePath, int flag) {
	image = cv::imread(imagePath, flag);
}

void Image::save(const std::string& imagePath) {
	cv::imwrite(imagePath, image);
}

void Image::loadImage(int64_t imageKey) {
	std::string imageHash;
	getImageHash(&imageHash, imageKey);
	load(GlobalConfig::IMAGE_DIRECTORY + imageHash + ".jpg");
}

void Image::loadCropImage(int64_t imageKey) {
	std::string imageHash;
	getImageHash(&imageHash, imageKey);
	load(GlobalConfig::IMAGE_CROP_DIRECTORY + imageHash + ".jpg");
}
void Image::saveCropImage(int64_t imageKey) {
	std::string imageHash;
	getImageHash(&imageHash, imageKey);
	save(GlobalConfig::IMAGE_CROP_DIRECTORY + imageHash + ".jpg");
}
void Image::loadMaskImage(int64_t imageKey) {
	std::string imageHash;
	getImageHash(&imageHash, imageKey);
	load(GlobalConfig::IMAGE_MASK_DIRECTORY + imageHash + ".png", 0);
}
void Image::saveMaskImage(int64_t imageKey) {
	image *= 255;
	std::string imageHash;
	getImageHash(&imageHash, imageKey);
	save(GlobalConfig::IMAGE_MASK_DIRECTORY + imageHash + ".png");
}

void Image::getImageHash(std::string* pImageHash, int64_t imageKey) {
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	dbAdapter->loadCell(pImageHash, GlobalConfig::IMAGE_TABLE, imageKey,
			GlobalConfig::IMAGE_HASH_COLUMN);
}

bool Image::empty() {
	return image.empty();
}

// deprecated functions

void Image::load(int64_t rowKey) {
	loadImage(rowKey);
}

//void Image::uniqueId() {
//	std::string strMaxId;
//	MongoDBAdapter::instance()->loadCell(&strMaxId, "max_id", 1, "cf:max_id");
//	int64_t nextId = 1;
//	if (!strMaxId.empty()) {
//		TypeConverter::readStringToNum(&mId, strMaxId);
//		nextId = mId + 1;
//	} else {
//		mId = 0;
//	}
//	TypeConverter::readNumToString(&strMaxId, nextId);
//	MongoDBAdapter::instance()->saveCell(strMaxId, "max_id", 1, "cf:max_id");
//}
//
//void Image::loadFromHbase(const std::string& imageHash) {
//	// load from Hbase
//	std::string strImageData;
//	HbaseAdapter::instance()->loadCell(&strImageData, GlobalConfig::IMAGE_TABLE,
//			imageHash, GlobalConfig::IMAGE_DATA_COLUMN);
//	std::vector<char> buf(strImageData.begin(), strImageData.end());
//	image = cv::imdecode(cv::Mat(buf), 0);
//	mImageHash = imageHash;
//	uniqueId();
//	// save to MongoDB
//	MongoDBAdapter::instance()->saveFile(strImageData, imageHash + ".jpg");
//	MongoDBAdapter::instance()->saveCell(imageHash, GlobalConfig::IMAGE_TABLE,
//			mId, GlobalConfig::IMAGE_HASH_COLUMN);
//}
