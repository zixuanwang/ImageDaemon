/*
 * DBAdapter.h
 *
 *  Created on: Jan 13, 2012
 *      Author: zixuan
 */

#ifndef DBADAPTER_H_
#define DBADAPTER_H_

#include "GlobalConfig.h"
class DBAdapter {
public:
	DBAdapter();
	virtual ~DBAdapter();
	virtual void loadCell(std::string* pString, const std::string& tableName,
			int64_t rowKey, const std::string& columnName)=0;
	virtual void loadCells(std::vector<std::string>* pStringArray,
			const std::string& tableName, int64_t rowKey,
			const std::vector<std::string>& columnNameArray)=0;
	virtual void saveCell(const std::string& string,
			const std::string& tableName, int64_t rowKey,
			const std::string& columnName)=0;
	virtual void loadCell(std::string* pString, const std::string& tableName,
			const std::string& rowKey, const std::string& columnName)=0;
	virtual void saveCell(const std::string& string,
			const std::string& tableName, const std::string& rowKey,
			const std::string& columnName)=0;

	virtual int scannerOpen(const std::string& tableName,
			const std::string& startRow,
			const std::vector<std::string>& columnNameArray)=0;
	virtual void scannerGet(std::string* pString,
			std::map<std::string, std::string>* pStringMap, int scanner)=0;
	virtual void scannerClose(int scanner)=0;
};

#endif /* DBADAPTER_H_ */
