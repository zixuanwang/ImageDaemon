#include "GlobalConfig.h"
#include "ANNTreeDaemon.h"
#include "ANNTreeRoot.h"
#include "ANNTreeSlave.h"
#include "HbaseAdapter.h"
#include "ANNTreeHelper.h"
#include "test.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

using namespace ::net::walnutvision;

class ANNTreeDaemonHandler: virtual public ANNTreeDaemonIf {
public:
	ANNTreeDaemonHandler(const std::string& confPath, bool root,
			int slavePort) {
		if (root) {
			ANNTreeRoot::instance()->init(confPath, slavePort);
			ANNTreeHelper::instance()->computeYPTree();
		}
	}

	/// Clear all data in root and all slaves
	void clear() {
		ANNTreeRoot::instance()->clear();
	}

	int32_t addTree() {
		return ANNTreeRoot::instance()->addTree();
	}

	void loadSample(const int32_t treeIndex, const std::string& sampleArray,
			const int32_t sampleCount) {
		std::vector<float> fSampleArray;
		TypeConverter::readStringToArray(&fSampleArray, sampleArray);
		ANNTreeRoot::instance()->loadSample(treeIndex, fSampleArray,
				sampleCount);
	}

	void addFeature(const int32_t treeIndex, const int64_t id,
			const std::string& feature) {
		std::vector<float> fFeature;
		TypeConverter::readStringToArray(&fFeature, feature);
		ANNTreeRoot::instance()->addFeature(treeIndex, id, fFeature);
	}

	void index(const int32_t treeIndex) {
		ANNTreeRoot::instance()->index(treeIndex);
	}

	void knnSearch(std::vector<Neighbor> & _return, const int32_t treeIndex,
			const std::string& feature, const int32_t k) {
		std::cout << "RPC knnSearch" << std::endl;
		std::vector<float> fFeature;
		TypeConverter::readStringToArray(&fFeature, feature);
		ANNTreeRoot::instance()->knnSearch(&_return, treeIndex, fFeature, k);
	}

	void similarSearch(std::vector<Neighbor> & _return, const int32_t treeIndex,
			const int64_t id, const int32_t k) {
		std::cout << "RPC similarSearch" << std::endl;
		ANNTreeHelper::instance()->similarSearch(&_return, treeIndex, id, k);
	}

	void slaveClear() {
		ANNTreeSlave::instance()->clear();
	}

	int32_t slaveAddTree() {
		return ANNTreeSlave::instance()->addTree();
	}

	void slaveInit(const int32_t treeIndex) {
		ANNTreeSlave::instance()->init(treeIndex);
	}

	void slaveAddFeature(const int32_t treeIndex, const int64_t id,
			const std::string& feature) {
		std::vector<float> fFeature;
		TypeConverter::readStringToArray(&fFeature, feature);
		ANNTreeSlave::instance()->addFeature(treeIndex, id, fFeature);
	}

	void slaveIndex(const int32_t treeIndex) {
		std::cout << "RPC slaveIndex" << std::endl;
		ANNTreeSlave::instance()->index(treeIndex);
	}

	void slaveKnnSearch(std::vector<Neighbor> & _return,
			const int32_t treeIndex, const std::string& feature,
			const int32_t k) {
		std::cout << "RPC slaveKnnSearch" << std::endl;
		std::vector<int64_t> neighborIdArray;
		std::vector<float> distArray;
		std::vector<float> fFeature;
		TypeConverter::readStringToArray(&fFeature, feature);
		ANNTreeSlave::instance()->knnSearch(&neighborIdArray, &distArray,
				treeIndex, fFeature, k);
		for (size_t i = 0; i < neighborIdArray.size(); ++i) {
			Neighbor neighbor;
			neighbor.id = neighborIdArray[i];
			neighbor.distance = (double) distArray[i];
			_return.push_back(neighbor);
		}
	}

};

int main(int argc, char *argv[]) {
	if (argc == 1) {
		std::cout << "Usage: ANNTreeDaemon -c <confPath> -r -p 9999"
				<< std::endl;
		return 0;
	}
	int port = 9999;
	std::string confPath;
	bool root = false;
	int c;
	while ((c = getopt(argc, argv, "c:rp:")) != -1) {
		switch (c) {
		case 'c':
			confPath = strdup(optarg);
			break;
		case 'r':
			root = true;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		}
	}
	if (confPath.empty()) {
		std::cout << "Usage: ANNTreeDaemon -c <confPath> -r -p 9999"
				<< std::endl;
		return 0;
	}
	shared_ptr<ANNTreeDaemonHandler> handler(
			new ANNTreeDaemonHandler(confPath, root, port - 1));
	shared_ptr<TProcessor> processor(new ANNTreeDaemonProcessor(handler));
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
	shared_ptr<ThreadManager> threadManager =
			ThreadManager::newSimpleThreadManager(50);
	shared_ptr<PosixThreadFactory> threadFactory =
			shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
	threadManager->threadFactory(threadFactory);
	threadManager->start();
	TNonblockingServer server(processor, protocolFactory, port, threadManager);
	server.serve();
	return 0;
}

