/*
 * MongoDBAdapter.cpp
 *
 *  Created on: Jan 13, 2012
 *      Author: zixuan
 */

#include "MongoDBAdapter.h"
MongoDBAdapter* MongoDBAdapter::pInstance = NULL;

MongoDBAdapter::MongoDBAdapter() {

}

MongoDBAdapter* MongoDBAdapter::instance() {
	if (pInstance == NULL) {
		pInstance = new MongoDBAdapter;
	}
	return pInstance;
}

void MongoDBAdapter::init() {
	try {
		ScopedDbConnection conn(GlobalConfig::MONGODB_SERVER);
		try {
			conn->ensureIndex(
					GlobalConfig::MONGO_DATABASE + "."
							+ GlobalConfig::IMAGE_TABLE, BSON("rowkey"<<1),
					true);
			conn->ensureIndex(
					GlobalConfig::MONGO_DATABASE + "."
							+ GlobalConfig::IMAGE_TABLE,
					BSON(GlobalConfig::IMAGE_HASH_COLUMN<<1), true);
			conn->ensureIndex(
					GlobalConfig::MONGO_DATABASE + "."
							+ GlobalConfig::INVERT_INDEX_TABLE,
					BSON("rowkey"<<1), true);
		} catch (DBException &e) {
			std::cerr << "Error in MongoDBAdapter initialization " << e.what()
					<< std::endl;
		}
		conn.done();
	} catch (DBException &e) {
		std::cerr << e.what() << std::endl;
	}
}

void MongoDBAdapter::loadCell(std::string* pString,
		const std::string& tableName, int64_t rowKey,
		const std::string& columnName) {
	pString->clear();
	try {
		ScopedDbConnection conn(GlobalConfig::MONGODB_SERVER);
		try {
			BSONObj fieldsToReturn = BSON(columnName<<1);
			auto_ptr<DBClientCursor> cursor = conn->query(
					GlobalConfig::MONGO_DATABASE + "." + tableName,
					QUERY("rowkey"<<(long long)rowKey), 1, 0,
					&fieldsToReturn);
			if (cursor->more()) {
				BSONElement e = cursor->next().getField(columnName);
				if (e.ok()) {
					int len;
					const char* ptr = e.binData(len);
					pString->assign(ptr, ptr + len);
				}
			}
		} catch (DBException &e) {
			std::cerr << "Error in loading cell " << e.what() << std::endl;
		}
		conn.done();
	} catch (DBException &e) {
		std::cerr << e.what() << std::endl;
	}
}

void MongoDBAdapter::loadCells(std::vector<std::string>* pStringArray,
		const std::string& tableName, int64_t rowKey,
		const std::vector<std::string>& columnNameArray) {
	pStringArray->clear();
	if (columnNameArray.empty()) {
		return;
	}
	try {
		ScopedDbConnection conn(GlobalConfig::MONGODB_SERVER);
		try {
			BSONObjBuilder builder;
			for (size_t i = 0; i < columnNameArray.size(); ++i) {
				builder << columnNameArray[i] << 1;
			}
			BSONObj fieldsToReturn = builder.obj();
			auto_ptr<DBClientCursor> cursor = conn->query(
					GlobalConfig::MONGO_DATABASE + "." + tableName,
					QUERY("rowkey"<<(long long)rowKey), 1, 0,
					&fieldsToReturn);
			if (cursor->more()) {
				pStringArray->assign(columnNameArray.size(), "");
				BSONObj obj = cursor->next();
				for (size_t i = 0; i < columnNameArray.size(); ++i) {
					BSONElement e = obj.getField(columnNameArray[i]);
					if (e.ok()) {
						int len;
						const char* ptr = e.binData(len);
						(*pStringArray)[i].assign(ptr, ptr + len);
					}
				}
			}
		} catch (DBException &e) {
			std::cerr << "Error in loading cells " << e.what() << std::endl;
		}
		conn.done();
	} catch (DBException &e) {
		std::cerr << e.what() << std::endl;
	}
}

void MongoDBAdapter::saveCell(const std::string& string,
		const std::string& tableName, int64_t rowKey,
		const std::string& columnName) {
	if (string.empty()) {
		return;
	}
	try {
		ScopedDbConnection conn(GlobalConfig::MONGODB_SERVER);
		try {
			BSONObj p = BSONObjBuilder().appendBinData(columnName,
					string.size(), BinDataGeneral, &string[0]).obj();
			conn->update(GlobalConfig::MONGO_DATABASE + "." + tableName,
					BSON("rowkey"<<(long long) rowKey), BSON( "$set" << p ),
					true);
		} catch (DBException &e) {
			std::cerr << "Error in saving cell " << e.what() << std::endl;
		}
		conn.done();
	} catch (DBException &e) {
		std::cerr << "Error in saving cell " << e.what() << std::endl;
	}

}

void MongoDBAdapter::saveFile(const std::string& string,
		const std::string& remoteName) {
	if (string.empty()) {
		return;
	}
	try {
		ScopedDbConnection conn(GlobalConfig::MONGODB_SERVER);
		try {
			GridFS gFS(conn.conn(), GlobalConfig::MONGO_DATABASE);
			gFS.storeFile((char*) &string[0], string.size(), remoteName);
		} catch (DBException &e) {
			std::cerr << "Error in saving file " << e.what() << std::endl;
		}
		conn.done();
	} catch (DBException &e) {
		std::cerr << e.what() << std::endl;
	}
}

void MongoDBAdapter::loadFile(std::string* pString,
		const std::string& remoteName) {
	pString->clear();
	try {
		ScopedDbConnection conn(GlobalConfig::MONGODB_SERVER);
		try {
			GridFS gFS(conn.conn(), GlobalConfig::MONGO_DATABASE);
			GridFile gf = gFS.findFile(remoteName);
			std::stringstream ss(
					std::stringstream::out | std::stringstream::binary);
			if (gf.exists()) {
				gf.write(ss);
				*pString = ss.str();
			}
		} catch (DBException &e) {
			std::cerr << "Error in loading file " << e.what() << std::endl;
		}
		conn.done();
	} catch (DBException &e) {
		std::cerr << e.what() << std::endl;
	}
}

bool MongoDBAdapter::checkExist(const std::string& tableName, int64_t rowKey,
		const std::string& columnName) {
	bool ret = false;
	try {
		ScopedDbConnection conn(GlobalConfig::MONGODB_SERVER);
		try {
			BSONObj fieldsToReturn = BSON(columnName<<1);
			auto_ptr<DBClientCursor> cursor = conn->query(
					GlobalConfig::MONGO_DATABASE + "." + tableName,
					QUERY("rowkey"<<(long long)rowKey), 1, 0,
					&fieldsToReturn);
			if (cursor->more()) {
				BSONElement e = cursor->next().getField(columnName);
				if (e.ok()) {
					ret = true;
				}
			}
		} catch (DBException &e) {
			std::cerr << "Error in checking exist " << e.what() << std::endl;
		}
		conn.done();
	} catch (DBException &e) {
		std::cerr << e.what() << std::endl;
	}
	return ret;
}
