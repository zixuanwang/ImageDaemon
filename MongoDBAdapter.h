/*
 * MongoDBAdapter.h
 *
 *  Created on: Jan 13, 2012
 *      Author: zixuan
 */

#ifndef MONGODBADAPTER_H_
#define MONGODBADAPTER_H_

#include "GlobalConfig.h"
#include "DBAdapter.h"

using namespace mongo;
class MongoDBAdapter: public DBAdapter {
public:
	static MongoDBAdapter* instance();
	void init();
	void loadCell(std::string* pString, const std::string& tableName,
			int64_t rowKey, const std::string& columnName);
	void loadCells(std::vector<std::string>* pStringArray,
			const std::string& tableName, int64_t rowKey,
			const std::vector<std::string>& columnNameArray);
	void saveCell(const std::string& string, const std::string& tableName,
			int64_t rowKey, const std::string& columnName);
	void saveFile(const std::string& string, const std::string& remoteName);
	void loadFile(std::string* pString, const std::string& remoteName);
	bool checkExist(const std::string& tableName, int64_t rowKey,
			const std::string& columnName = "rowkey");
private:
	static MongoDBAdapter* pInstance;
	// Default constuctor, copy constructor and assignment operators should be as private so that nobody
	// from outside can call those functions and instantiate it
	MongoDBAdapter();
	MongoDBAdapter(const MongoDBAdapter&);
	MongoDBAdapter& operator=(const MongoDBAdapter&);
};

#endif /* MONGODBADAPTER_H_ */
