/*
 * HbaseAdapter.h
 *
 *  Created on: Jan 7, 2012
 *      Author: zixuan
 */

#ifndef HBASEADAPTER_H_
#define HBASEADAPTER_H_
#include "GlobalConfig.h"
#include "TypeConverter.h"
#include "DBAdapter.h"
#include "Hbase.h"
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::hadoop::hbase::thrift;
class HbaseAdapter: public DBAdapter {
public:
//	static HbaseAdapter* instance();
	HbaseAdapter();
	~HbaseAdapter();
	void loadCell(std::string* pString, const std::string& tableName,
			int64_t rowKey, const std::string& columnName);
	void loadCells(std::vector<std::string>* pStringArray,
			const std::string& tableName, int64_t rowKey,
			const std::vector<std::string>& columnNameArray);
	void saveCell(const std::string& string, const std::string& tableName,
			int64_t rowKey, const std::string& columnName);
	void loadCell(std::string* pString, const std::string& tableName,
			const std::string& rowKey, const std::string& columnName);
	void saveCell(const std::string& string, const std::string& tableName,
			const std::string& rowKey, const std::string& columnName);

	int scannerOpen(const std::string& tableName, const std::string& startRow,
			const std::vector<std::string>& columnNameArray);
	void scannerGet(std::string* pString,
			std::map<std::string, std::string>* pStringMap, int scanner);
	void scannerClose(int scanner);
private:
	boost::shared_ptr<TSocket> mSocket;
	boost::shared_ptr<TTransport> mTransport;
	boost::shared_ptr<TProtocol> mProtocol;
	boost::shared_ptr<HbaseClient> mClient;
};

#endif /* HBASEADAPTER_H_ */
