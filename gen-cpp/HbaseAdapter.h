/*
 * HbaseAdapter.h
 *
 *  Created on: Jan 7, 2012
 *      Author: zixuan
 */

#ifndef HBASEADAPTER_H_
#define HBASEADAPTER_H_
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>

#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <opencv2/opencv.hpp>
#include "GlobalConfig.h"
#include "TypeConverter.h"
#include "Hbase.h"
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace apache::hadoop::hbase::thrift;
class HbaseAdapter {
public:
	static HbaseAdapter* instance();
	void init();
	void loadCell(std::string* pString, const std::string& tableName, const std::string& rowKey, const std::string& columnName);
	void saveCell(const std::string& string, const std::string& tableName, const std::string& rowKey, const std::string& columnName);
private:
	boost::shared_ptr<HbaseClient> mpHbaseClient;
	static HbaseAdapter* pInstance;
	// Default constuctor, copy constructor and assignment operators should be as private so that nobody
	// from outside can call those functions and instantiate it
	HbaseAdapter();
	HbaseAdapter(const HbaseAdapter&);
	HbaseAdapter& operator=(const HbaseAdapter&);
};

#endif /* HBASEADAPTER_H_ */
