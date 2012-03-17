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

void Image::load(int64_t rowKey) {
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	std::string strImageHash;
	dbAdapter->loadCell(&strImageHash, GlobalConfig::IMAGE_TABLE, rowKey,
			GlobalConfig::IMAGE_HASH_COLUMN);
	image = cv::imread(GlobalConfig::IMAGE_DIRECTORY + strImageHash + ".jpg",
			1);
}

void Image::load(const std::string& imageHash){
	image = cv::imread(GlobalConfig::IMAGE_DIRECTORY + imageHash + ".jpg",
			1);
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
