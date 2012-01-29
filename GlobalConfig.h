/*
 * GlobalConfig.h
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#ifndef GLOBALCONFIG_H_
#define GLOBALCONFIG_H_

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <opencv2/opencv.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/shared_ptr.hpp>

#include <mongo/client/dbclient.h>
#include <mongo/client/gridfs.h>
#include <mongo/client/connpool.h>
class GlobalConfig {
public:
	GlobalConfig();
	~GlobalConfig();
	const static std::string MONGO_DATABASE;
	const static std::string IMAGE_TABLE;
	const static std::string INVERT_INDEX_TABLE;
	const static std::string COLUMN_FAMILY;
	const static std::string IMAGE_DATA_COLUMN;
	const static std::string IMAGE_HASH_COLUMN;
	const static std::string IMAGE_FEATURE_COLUMN;
	const static std::string IMAGE_COLOR_FEATURE_COLUMN;
	const static std::string IMAGE_BOW_HISTOGRAM_COLUMN;
	const static std::string INVERT_INDEX_SHORT_COLUMN;
	const static std::string INVERT_INDEX_LONG_COLUMN;
	const static std::string INVERT_INDEX_SHORT_SCORE_COLUMN;
	const static std::string INVERT_INDEX_LONG_SCORE_COLUMN;
	const static std::string MONGODB_SERVER;
	const static int SHORT_LIST_SIZE;
	const static int IMAGE_LENGTH;
};

#endif /* GLOBALCONFIG_H_ */
