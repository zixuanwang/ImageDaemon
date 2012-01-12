/*
 * HbaseAdapter.cpp
 *
 *  Created on: Jan 7, 2012
 *      Author: zixuan
 */

#include "HbaseAdapter.h"
HbaseAdapter* HbaseAdapter::pInstance = NULL;

HbaseAdapter::HbaseAdapter() {

}

HbaseAdapter* HbaseAdapter::instance() {
	if (pInstance == NULL) {
		pInstance = new HbaseAdapter;
	}
	return pInstance;
}

void HbaseAdapter::init() {
	boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	mpHbaseClient.reset(new HbaseClient(protocol));
	transport->open();
	// create tables
	std::vector<ColumnDescriptor> columns;
	columns.push_back(ColumnDescriptor());
	columns.back().name = GlobalConfig::COLUMN_FAMILY;
	columns.back().maxVersions = 1;
	try {
		mpHbaseClient->createTable(GlobalConfig::IMAGE_TABLE, columns);
	} catch (const AlreadyExists &ae) {

	}
	try {
		mpHbaseClient->createTable(GlobalConfig::INVERT_INDEX_TABLE, columns);
	} catch (const AlreadyExists &ae) {

	}
}

void HbaseAdapter::loadCell(std::string* pString, const std::string& tableName,
		const std::string& rowKey, const std::string& columnName) {
	pString->clear();
	try {
		std::vector<TRowResult> value;
		std::vector<std::string> columnNameArray;
		columnNameArray.push_back(columnName);
		mpHbaseClient->getRowWithColumns(value, tableName, rowKey,
				columnNameArray);
		for (size_t i = 0; i < value.size(); i++) {
			for (std::map<std::string, TCell>::const_iterator it =
					value[i].columns.begin(); it != value[i].columns.end();
					++it) {
				*pString = it->second.value;
			}
		}

	} catch (const TException& tx) {
		std::cerr << tx.what() << std::endl;
	}
}
void HbaseAdapter::saveCell(const std::string& string,
		const std::string& tableName, const std::string& rowKey,
		const std::string& columnName) {
	try {
		std::vector<Mutation> mutations;
		mutations.push_back(Mutation());
		mutations.back().column = columnName;
		mutations.back().value = string;
		mpHbaseClient->mutateRow(tableName, rowKey, mutations);
	} catch (const TException& tx) {
		std::cerr << tx.what() << std::endl;
	}
}
