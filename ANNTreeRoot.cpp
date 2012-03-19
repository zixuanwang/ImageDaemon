/*
 * ANNTreeRoot.cpp
 *
 *  Created on: Feb 19, 2012
 *      Author: zixuan
 */

#include "ANNTreeRoot.h"
ANNTreeRoot* ANNTreeRoot::pInstance = NULL;
pthread_mutex_t ANNTreeRoot::sMutex = PTHREAD_MUTEX_INITIALIZER;

ANNTreeRoot::ANNTreeRoot() {

}

ANNTreeRoot* ANNTreeRoot::instance() {
	if (pInstance == NULL) {
		pInstance = new ANNTreeRoot;
	}
	return pInstance;
}

void ANNTreeRoot::init(const std::string& confPath, int slavePort) {
	loadConfiguration(confPath);
	if (mSlaveNameArray.empty()) {
		std::cerr << "Empty slaves, please load configuration first."
				<< std::endl;
		return;
	}
	// start slave processes
	std::string cmd = "parallel-ssh -h " + mConfPath
			+ " -t 0 \"ANNTreeDaemon -s -p "
			+ boost::lexical_cast<std::string>(slavePort)
			+ " > /tmp/slave.log\" &";
	if (system(cmd.c_str()) != 0) {
		std::cerr << "Error in starting ANNTreeSlave." << std::endl;
	}
	std::cout << cmd << std::endl;
	sleep(3);
	// connect to slaves
	for (size_t i = 0; i < mSlaveNameArray.size(); ++i) {
		try {
			boost::shared_ptr<TSocket> socket(
					new TSocket(mSlaveNameArray[i], slavePort));
			boost::shared_ptr<TTransport> transport(
					new TFramedTransport(socket));
			boost::shared_ptr<TProtocol> protocol(
					new TBinaryProtocol(transport));
			mClientArray.push_back(
					boost::shared_ptr<ANNTreeDaemonClient>(
							new ANNTreeDaemonClient(protocol)));
			transport->open();
		} catch (const TException &tx) {
			std::cerr << tx.what() << std::endl;
		}
	}
}

void ANNTreeRoot::loadConfiguration(const std::string& confPath) {
	std::ifstream inStream;
	inStream.open(confPath.c_str());
	if (inStream.good()) {
		std::string server;
		while (getline(inStream, server)) {
			mSlaveNameArray.push_back(server);
		}
		inStream.close();
		mConfPath = confPath;
	}
}

void ANNTreeRoot::build() {
	if (mSlaveNameArray.empty()) {
		std::cerr << "Empty slaves, please load configuration first."
				<< std::endl;
		return;
	}
	selectSeed(10000);
	try {
		boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
		boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
		boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
		HbaseClient client(protocol);
		transport->open();
		std::vector<std::string> columnNameArray;
		columnNameArray.push_back(GlobalConfig::IMAGE_COLOR_FEATURE_COLUMN);
		int scanner = client.scannerOpen(GlobalConfig::IMAGE_TABLE, "",
				columnNameArray);
		while (true) {
			std::vector<TRowResult> value;
			client.scannerGet(value, scanner);
			if (value.empty()) {
				break;
			}
			for (size_t i = 0; i < value.size(); ++i) {
				int64_t rowKey;
				TypeConverter::readStringToNum(&rowKey, value[i].row);
				for (std::map<std::string, TCell>::const_iterator iter =
						value[i].columns.begin();
						iter != value[i].columns.end(); ++iter) {
					std::vector<float> featureVector;
					TypeConverter::readStringToArray(&featureVector,
							iter->second.value);
					int k = 1;
					std::vector<int> indices(k);
					std::vector<float> dists(k);
					mpIndex->knnSearch(featureVector, indices, dists, k,
							cv::flann::SearchParams(32));
					int slaveSize = (int) mClientArray.size();
					mClientArray[indices[0] % slaveSize]->slaveAddFeature(
							rowKey, iter->second.value);
				}
			}
		}
		for (size_t i = 0; i < mSlaveNameArray.size(); ++i) {
			mClientArray[i]->slaveIndex();
		}
		transport->close();
	} catch (const TException &tx) {
		std::cerr << tx.what() << std::endl;
	}
}

int ANNTreeRoot::selectSeed(int sampleCount) {
	int row = 0;
	if (sampleCount <= 0) {
		return 0;
	}
	try {
		boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
		boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
		boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
		HbaseClient client(protocol);
		transport->open();
		std::vector<std::string> columnNameArray;
		columnNameArray.push_back(GlobalConfig::IMAGE_COLOR_FEATURE_COLUMN);
		int scanner = client.scannerOpen(GlobalConfig::IMAGE_TABLE, "",
				columnNameArray);
		srand(time(NULL));
		std::vector<float> data;
		while (true) {
			std::vector<TRowResult> value;
			client.scannerGet(value, scanner);
			if (value.empty()) {
				break;
			}
			if (row < sampleCount
					|| (float) rand() / RAND_MAX < (float) sampleCount / row) {
				std::vector<float> featureVector;
				for (size_t i = 0; i < value.size(); ++i) {
					for (std::map<std::string, TCell>::const_iterator iter =
							value[i].columns.begin();
							iter != value[i].columns.end(); ++iter) {
						TypeConverter::readStringToArray(&featureVector,
								iter->second.value);
					}
				}
				if (row < sampleCount) {
					// add first s samples to data
					std::copy(featureVector.begin(), featureVector.end(),
							std::back_inserter(data));
				} else {
					// randomly select one to drop
					int dropIndex = rand() % sampleCount;
					size_t dataLength = data.size();
					int dataDimension = dataLength / sampleCount;
					std::vector<float>::iterator iter = data.begin()
							+ dropIndex * dataDimension;
					std::copy(featureVector.begin(), featureVector.end(), iter);
				}
			}
			++row;
		}
		mMat = cv::Mat(data, true);
		mMat = mMat.reshape(0, row);
		mpIndex.reset(
				new cv::flann::Index(mMat, cv::flann::KDTreeIndexParams(8)));
		transport->close();
	} catch (const TException &tx) {
		std::cerr << tx.what() << std::endl;
	}
	return row;
}

void ANNTreeRoot::addFeature(int64_t id, const std::string& feature) {
//	std::vector<int> slaveArray;
//	getSlave(&slaveArray, feature);
//	for (size_t i = 0; i < slaveArray.size(); ++i) {
//		mClientArray[slaveArray[i]]->slaveAddFeature(id,feature);
//	}
}

void ANNTreeRoot::knnSearch(std::vector<Neighbor>* pReturn,
		const std::string& feature, int k) {
	pReturn->clear();
	std::vector<int> slaveArray;
	getSlave(&slaveArray, feature);
	for (size_t i = 0; i < slaveArray.size(); ++i) {
		mClientArray[slaveArray[i]]->slaveKnnSearch(*pReturn, feature, k);
	}
}

// TODO: return more than 1 slaves
void ANNTreeRoot::getSlave(std::vector<int>* pSlaveArray,
		const std::string& feature) {
	pSlaveArray->clear();
	std::vector<float> fFeature;
	TypeConverter::readStringToArray(&fFeature, feature);
	std::vector<int> indices(1);
	std::vector<float> dists(1);
	mpIndex->knnSearch(fFeature, indices, dists, 1,
			cv::flann::SearchParams(32));
	int slaveSize = (int) mClientArray.size();
	pSlaveArray->push_back(indices[0] % slaveSize);
}

void ANNTreeRoot::buildFeature(const std::string& type,
		const std::string& featureDirectory, const std::string& imageDirectory,
		const std::string& listDirectory) {
	/// load feature
	boost::unordered_map<int, std::vector<float> > featureMap;
	boost::filesystem::directory_iterator end_itr;
	for (boost::filesystem::directory_iterator itr(featureDirectory);
			itr != end_itr; ++itr) {
		if (!boost::filesystem::is_directory(itr->status())) {
			std::string filePath = itr->path().string();
			std::ifstream inStream;
			inStream.open(filePath.c_str());
			if (inStream.good()) {
				std::string line;
				int lineCount = 0;
				while (getline(inStream, line)) {
					std::vector<std::string> stringArray;
					boost::split(stringArray, line, boost::is_any_of(","));
					for (size_t i = 0; i < stringArray.size(); ++i) {
						featureMap[lineCount].push_back(
								atof(stringArray[i].c_str()));
					}
					++lineCount;
				}
				inStream.close();
			}
		}
	}
	std::cout << "There are " << featureMap.size() << " samples." << std::endl;
	MYSQL *connect; // Create a pointer to the MySQL instance
	connect = mysql_init(NULL);
	connect = mysql_real_connect(connect, "localhost", "root", "myhome515",
			"face", 0, NULL, 0);
	std::string sqlString =
			"CREATE TABLE " + type
					+ " (id INT, path varchar(255) NOT NULL, feature MEDIUMBLOB, PRIMARY KEY(id), INDEX(path))";
	mysql_query(connect, sqlString.c_str());
	std::ifstream inStream;
	inStream.open(listDirectory.c_str());
	if (inStream.good()) {
		std::string line;
		int i = 0;
		while (getline(inStream, line)) {
			std::string imagePath = imageDirectory + "/" + line;
			char chunk[2 * 1000 * 1024 + 1];
			char query[1024 * 5000];
			mysql_real_escape_string(connect, chunk, (char*) &featureMap[i][0],
					sizeof(float) * featureMap[i].size());
			std::string sqlString = "INSERT INTO " + type
					+ " (id, path, feature) VALUES('%d', '%s', '%s')";
			int len = snprintf(query, sizeof(query), sqlString.c_str(), i,
					imagePath.c_str(), chunk);
			mysql_real_query(connect, query, len);
			++i;
		}
		inStream.close();
	}
	mysql_close(connect); /* Close and shutdown */

	// build root index
	std::vector<float> data;
	std::set<int> indexSet;
	while (indexSet.size() < 100) {
		indexSet.insert(rand() % featureMap.size());
	}
	for (std::set<int>::iterator iter = indexSet.begin();
			iter != indexSet.end(); ++iter) {
		std::copy(featureMap[*iter].begin(), featureMap[*iter].end(),
				std::back_inserter(data));
	}
	mMat = cv::Mat(data, true);
	mMat = mMat.reshape(0, indexSet.size());
	mpIndex.reset(new cv::flann::Index(mMat, cv::flann::KDTreeIndexParams(8)));
	for (boost::unordered_map<int, std::vector<float> >::iterator iter =
			featureMap.begin(); iter != featureMap.end(); ++iter) {
		std::string stringFeature;
		TypeConverter::readArrayToString(&stringFeature, iter->second);
		// TODO: only one slave
		mClientArray[0]->slaveAddFeature(iter->first, stringFeature);
	}
	mClientArray[0]->slaveIndex();
}

void ANNTreeRoot::similarSearch(std::vector<Neighbor>* pReturn, int64_t id,
		int k, const std::string& type) {
	pReturn->clear();
	MYSQL *connect;
	connect = mysql_init(NULL);
	connect = mysql_real_connect(connect, "localhost", "root", "myhome515",
			"face", 0, NULL, 0);
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned long *lengths;
	std::string queryString = "SELECT feature FROM " + type + " WHERE id = "
			+ boost::lexical_cast<std::string>(id);
	if (mysql_query(connect, queryString.c_str()) != 0) {
		return;
	}
	result = mysql_store_result(connect);
	row = mysql_fetch_row(result);
	lengths = mysql_fetch_lengths(result);
	if (lengths[0] > 0) {
		std::vector<float> feature(lengths[0] / sizeof(float));
		memcpy((char*) &feature[0], row[0], lengths[0]);
		std::string stringFeature;
		TypeConverter::readArrayToString(&stringFeature, feature);
		knnSearch(pReturn, stringFeature, k);
	}
	mysql_free_result(result);
	mysql_close(connect);
}
