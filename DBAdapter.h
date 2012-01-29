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
	virtual void init()=0;
	virtual void loadCell(std::string* pString, const std::string& tableName,
			int64_t rowKey, const std::string& columnName)=0;
	virtual void saveCell(const std::string& string,
			const std::string& tableName, int64_t rowKey,
			const std::string& columnName)=0;
};

#endif /* DBADAPTER_H_ */
