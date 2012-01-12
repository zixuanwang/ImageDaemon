/*
 * GlobalConfig.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#include "GlobalConfig.h"

const std::string GlobalConfig::IMAGE_TABLE = "image";
const std::string GlobalConfig::IMAGE_HASH_TABLE = "image_hash";
const std::string GlobalConfig::META_TABLE = "meta";
const std::string GlobalConfig::META_HASH_TABLE = "meta_hash";
const std::string GlobalConfig::ITEM_UPDATE_TABLE = "item_update";
const std::string GlobalConfig::INVERT_INDEX_TABLE = "invert_index";
const std::string GlobalConfig::COLUMN_FAMILY = "cf:";
const std::string GlobalConfig::IMAGE_DATA_COLUMN = COLUMN_FAMILY + "imd";
const std::string GlobalConfig::IMAGE_FEATURE_COLUMN = COLUMN_FAMILY + "imf";
const std::string GlobalConfig::IMAGE_BOW_HISTOGRAM_COLUMN = COLUMN_FAMILY
		+ "imbh";
const std::string GlobalConfig::INVERT_INDEX_SHORT_COLUMN = COLUMN_FAMILY
		+ "sl";
const std::string GlobalConfig::INVERT_INDEX_LONG_COLUMN = COLUMN_FAMILY + "ll";
const std::string GlobalConfig::INVERT_INDEX_SHORT_SCORE_COLUMN = COLUMN_FAMILY
		+ "ss";
const std::string GlobalConfig::INVERT_INDEX_LONG_SCORE_COLUMN = COLUMN_FAMILY
		+ "ls";
const int GlobalConfig::SHORT_LIST_SIZE = 3;

GlobalConfig::GlobalConfig() {
	// TODO Auto-generated constructor stub

}

GlobalConfig::~GlobalConfig() {
	// TODO Auto-generated destructor stub
}

