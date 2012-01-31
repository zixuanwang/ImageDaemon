/*
 * GlobalConfig.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#include "GlobalConfig.h"
const std::string GlobalConfig::MONGODB_SERVER = "zixuanpc";
const std::string GlobalConfig::MONGO_DATABASE = "image";
const std::string GlobalConfig::IMAGE_TABLE = "image";
const std::string GlobalConfig::INVERT_INDEX_TABLE = "invert_index";
const std::string GlobalConfig::COLUMN_FAMILY = "cf:";
const std::string GlobalConfig::IMAGE_HASH_COLUMN = COLUMN_FAMILY + "hash";
const std::string GlobalConfig::IMAGE_DATA_COLUMN = COLUMN_FAMILY + "imageData";
const std::string GlobalConfig::IMAGE_FEATURE_COLUMN = COLUMN_FAMILY
		+ "imageFeature";
const std::string GlobalConfig::IMAGE_COLOR_FEATURE_COLUMN = COLUMN_FAMILY
		+ "imageColorFeature";
const std::string GlobalConfig::IMAGE_BOW_HISTOGRAM_COLUMN = COLUMN_FAMILY
		+ "imageBoWHistogram";
const std::string GlobalConfig::INVERT_INDEX_SHORT_COLUMN = COLUMN_FAMILY
		+ "shortList";
const std::string GlobalConfig::INVERT_INDEX_LONG_COLUMN = COLUMN_FAMILY
		+ "longList";
const std::string GlobalConfig::INVERT_INDEX_SHORT_SCORE_COLUMN = COLUMN_FAMILY
		+ "shortScore";
const std::string GlobalConfig::INVERT_INDEX_LONG_SCORE_COLUMN = COLUMN_FAMILY
		+ "longScore";
const int GlobalConfig::SHORT_LIST_SIZE = 100;
const int GlobalConfig::IMAGE_LENGTH = 320;

GlobalConfig::GlobalConfig() {
	// TODO Auto-generated constructor stub

}

GlobalConfig::~GlobalConfig() {
	// TODO Auto-generated destructor stub
}

