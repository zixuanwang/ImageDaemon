/*
 * HbaseAdapter.cpp
 *
 *  Created on: Jan 7, 2012
 *      Author: zixuan
 */

#include "HbaseAdapter.h"

HbaseAdapter::HbaseAdapter() {
	try {
		mSocket.reset(new TSocket("localhost", 9090));
		mTransport.reset(new TBufferedTransport(mSocket));
		mProtocol.reset(new TBinaryProtocol(mTransport));
		mClient.reset(new HbaseClient(mProtocol));
		mTransport->open();
	} catch (const TException &tx) {
		std::cerr << "Thrift opening error " << tx.what() << std::endl;
	}
}


HbaseAdapter::~HbaseAdapter() {
	mTransport->close();
}

void HbaseAdapter::loadCell(std::string* pString, const std::string& tableName,
		int64_t rowKey, const std::string& columnName) {
	std::string strRowKey;
	TypeConverter::readNumToString(&strRowKey, rowKey);
	loadCell(pString, tableName, strRowKey, columnName);
}

void HbaseAdapter::loadCells(std::vector<std::string>* pStringArray,
		const std::string& tableName, int64_t rowKey,
		const std::vector<std::string>& columnNameArray) {
	pStringArray->clear();
	if (columnNameArray.empty()) {
		return;
	}
	try {
		std::vector<TRowResult> value;
		std::string strRowKey;
		TypeConverter::readNumToString(&strRowKey, rowKey);
		mClient->getRowWithColumns(value, tableName, strRowKey,
				columnNameArray);
		for (size_t i = 0; i < value.size(); i++) {
			for (std::map<std::string, TCell>::const_iterator it =
					value[i].columns.begin(); it != value[i].columns.end();
					++it) {
				pStringArray->push_back(it->second.value);
			}
		}

	} catch (const TException& tx) {
		std::cerr << tx.what() << std::endl;
	}
}

void HbaseAdapter::saveCell(const std::string& string,
		const std::string& tableName, int64_t rowKey,
		const std::string& columnName) {
	std::string strRowKey;
	TypeConverter::readNumToString(&strRowKey, rowKey);
	saveCell(string, tableName, strRowKey, columnName);
}

void HbaseAdapter::loadCell(std::string* pString, const std::string& tableName,
		const std::string& rowKey, const std::string& columnName) {
	pString->clear();
	try {
		std::vector<TCell> value;
		mClient->get(value, tableName, rowKey, columnName);
		for (size_t i = 0; i < value.size(); i++) {
			*pString = value[i].value;
		}

	} catch (const TException& tx) {
		std::cerr << tx.what() << std::endl;
	}
}
void HbaseAdapter::saveCell(const std::string& string,
		const std::string& tableName, const std::string& rowKey,
		const std::string& columnName) {
	if (string.empty()) {
		return;
	}
	try {
		std::vector<Mutation> mutations;
		mutations.push_back(Mutation());
		mutations.back().column = columnName;
		mutations.back().value = string;
		mClient->mutateRow(tableName, rowKey, mutations);
	} catch (const TException& tx) {
		std::cerr << tx.what() << std::endl;
	}
}

int HbaseAdapter::scannerOpen(const std::string& tableName,
		const std::string& startRow,
		const std::vector<std::string>& columnNameArray) {
	int scanner = -1;
	try {
		scanner = mClient->scannerOpen(tableName, startRow, columnNameArray);
	} catch (const TException& tx) {
		std::cerr << tx.what() << std::endl;
	}
	return scanner;
}

void HbaseAdapter::scannerGet(std::string* pString,
		std::map<std::string, std::string>* pStringMap, int scanner) {
	pString->clear();
	pStringMap->clear();
	try {
		std::vector<TRowResult> value;
		mClient->scannerGet(value, scanner);
		for (size_t i = 0; i < value.size(); ++i) {
			*pString = value[i].row;
			for (std::map<std::string, TCell>::const_iterator it =
					value[i].columns.begin(); it != value[i].columns.end();
					++it) {
				(*pStringMap)[it->first] = it->second.value;
			}
		}
	} catch (const IOError &ioe) {
		std::cerr << "FATAL: Scanner raised IOError"  << std::endl;
	} catch (const TException& tx) {
		std::cerr << tx.what() << std::endl;
	}
}
void HbaseAdapter::scannerClose(int scanner) {
	try {
		mClient->scannerClose(scanner);
	} catch (const TException& tx) {
		std::cerr << tx.what() << std::endl;
	}
}
