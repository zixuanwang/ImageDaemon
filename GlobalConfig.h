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
#include <set>
#include <map>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <pthread.h>
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/basicconfigurator.h>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <server/TNonblockingServer.h>
#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include <mysql/mysql.h>
#include <tbb/tbb.h>

class GlobalConfig {
public:
	GlobalConfig();
	~GlobalConfig();
	const static std::string IMAGE_TABLE;
	const static std::string INVERT_INDEX_TABLE;
	const static std::string CATEGORY_INDEX_TABLE;
	const static std::string COLUMN_FAMILY;
	const static std::string IMAGE_HASH_COLUMN;
	const static std::string IMAGE_FEATURE_COLUMN;
	const static std::string IMAGE_COLOR_FEATURE_COLUMN;
	const static std::string IMAGE_SHAPE_FEATURE_COLUMN;
	const static std::string IMAGE_SURF_FEATURE_COLUMN;
	const static std::string IMAGE_BOW_HISTOGRAM_COLUMN;
	const static std::string BUILD_TREEINDEX_COLUMN;
	const static std::string COLOR_TREEINDEX_COLUMN;
	const static std::string SHAPE_TREEINDEX_COLUMN;
	const static std::string SURF_TREEINDEX_COLUMN;
	const static std::string IMAGE_DIRECTORY;
	const static int VOCABULARY_SIZE;
	const static int SHORT_LIST_SIZE;
	const static int IMAGE_LENGTH;
	const static int CANDIDATE_COUNT;
	const static int VERIFICATION_COUNT;
	const static int STATIC_RANK_SIZE;
	const static int STATIC_RANK_SIZE_PER_PAGE;
	const static int SURF_FEATURE_COUNT_PER_IMAGE;
	const static int SURF_FEATURE_DIMENSION;
	const static int COLOR_FEATURE_BINSIZE;
	const static int SHAPE_FEATURE_BINSIZE;
	const static std::string INVERT_INDEX_NEXT_COLUMN;
	const static std::string INVERTED_INDEX_SERVER_NAME;
	const static int INVERTED_INDEX_SERVER_PORT;
	const static bool USE_VERIFICATION;
	const static float PI;

	// TODO: obsolete
	const static std::string MONGO_DATABASE;
	const static std::string MONGODB_SERVER;
	const static std::string INVERT_INDEX_SHORT_COLUMN;
	const static std::string INVERT_INDEX_LONG_COLUMN;
	const static std::string INVERT_INDEX_SHORT_SCORE_COLUMN;
	const static std::string INVERT_INDEX_LONG_SCORE_COLUMN;
};

#endif /* GLOBALCONFIG_H_ */
