/*
 * GlobalConfig.h
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#ifndef GLOBALCONFIG_H_
#define GLOBALCONFIG_H_
#include <string>
class GlobalConfig {
public:
	GlobalConfig();
	~GlobalConfig();
	const static std::string IMAGE_TABLE;
	const static std::string IMAGE_HASH_TABLE;
	const static std::string META_TABLE;
	const static std::string META_HASH_TABLE;
	const static std::string ITEM_UPDATE_TABLE;
	const static std::string INVERT_INDEX_TABLE;
	const static std::string COLUMN_FAMILY;
	const static std::string IMAGE_DATA_COLUMN;
	const static std::string IMAGE_FEATURE_COLUMN;
	const static std::string IMAGE_BOW_HISTOGRAM_COLUMN;
	const static std::string INVERT_INDEX_SHORT_COLUMN;
	const static std::string INVERT_INDEX_LONG_COLUMN;
	const static std::string INVERT_INDEX_SHORT_SCORE_COLUMN;
	const static std::string INVERT_INDEX_LONG_SCORE_COLUMN;
	const static int SHORT_LIST_SIZE;
};

#endif /* GLOBALCONFIG_H_ */
