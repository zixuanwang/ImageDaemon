#include "GlobalConfig.h"
#include "ANNTreeDaemon.h"
#include "ANNTreeRoot.h"
#include "ANNTreeSlavePool.h"
#include "ANNTreeRootPool.h"
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
			ANNTreeRoot::init(confPath, slavePort);
			ANNTreeHelper::instance()->buildAllCategory();
		}
	}

	/// delete all data in root and all slaves
	void clear() {
		ANNTreeRootPool::instance()->clear();
	}

	/// delete one rootTree and its corresponding slave trees
	void putTree(const int32_t treeIndex) {
		ANNTreeRootPool::instance()->put(treeIndex);
	}

	void loadSample(const int32_t treeIndex, const std::string& sampleArray,
			const int32_t sampleCount) {
		std::vector<float> fSampleArray;
		TypeConverter::readStringToArray(&fSampleArray, sampleArray);
		boost::shared_ptr<ANNTreeRoot> pRoot;
		ANNTreeRootPool::instance()->get(&pRoot, treeIndex);
		pRoot->loadSample(fSampleArray, sampleCount);
	}

	void addFeature(const int32_t treeIndex, const int64_t id,
			const std::string& feature) {
		std::vector<float> fFeature;
		TypeConverter::readStringToArray(&fFeature, feature);
		boost::shared_ptr<ANNTreeRoot> pRoot;
		ANNTreeRootPool::instance()->get(&pRoot, treeIndex);
		pRoot->addFeature(id, fFeature);
	}

	void index(const int32_t treeIndex) {
		boost::shared_ptr<ANNTreeRoot> pRoot;
		ANNTreeRootPool::instance()->get(&pRoot, treeIndex);
		pRoot->index();
	}

	void knnSearch(std::vector<Neighbor> & _return, const int32_t treeIndex,
			const std::string& feature, const int32_t k) {
		std::cout << "RPC knnSearch" << std::endl;
		std::vector<float> fFeature;
		TypeConverter::readStringToArray(&fFeature, feature);
		boost::shared_ptr<ANNTreeRoot> pRoot;
		ANNTreeRootPool::instance()->get(&pRoot, treeIndex);
		pRoot->knnSearch(&_return, fFeature, k);
	}

	void similarSearch(std::vector<Neighbor> & _return, const int32_t treeIndex,
			const int64_t id, const int32_t k) {
		std::cout << "RPC similarSearch" << std::endl;
		ANNTreeHelper::instance()->similarSearch(&_return, treeIndex, id, k);
	}

	/// delete one slaveTree
	void slavePutTree(const int32_t treeIndex) {
		ANNTreeSlavePool::instance()->put(treeIndex);
	}

	void slaveAddFeature(const int32_t treeIndex, const int64_t id,
			const std::string& feature) {
		std::vector<float> fFeature;
		TypeConverter::readStringToArray(&fFeature, feature);
		boost::shared_ptr<ANNTreeSlave> pSlave;
		ANNTreeSlavePool::instance()->get(&pSlave, treeIndex);
		pSlave->addFeature(id, fFeature);
	}

	void slaveIndex(const int32_t treeIndex) {
		std::cout << "RPC slaveIndex" << std::endl;
		boost::shared_ptr<ANNTreeSlave> pSlave;
		ANNTreeSlavePool::instance()->get(&pSlave, treeIndex);
		pSlave->index();
	}

	void slaveKnnSearch(std::vector<Neighbor> & _return,
			const int32_t treeIndex, const std::string& feature,
			const int32_t k) {
		std::cout << "RPC slaveKnnSearch" << std::endl;
		std::vector<int64_t> neighborIdArray;
		std::vector<float> distArray;
		std::vector<float> fFeature;
		TypeConverter::readStringToArray(&fFeature, feature);
		boost::shared_ptr<ANNTreeSlave> pSlave;
		ANNTreeSlavePool::instance()->get(&pSlave, treeIndex);
		pSlave->knnSearch(&neighborIdArray, &distArray, fFeature, k);
		for (size_t i = 0; i < neighborIdArray.size(); ++i) {
			Neighbor neighbor;
			neighbor.id = neighborIdArray[i];
			neighbor.distance = (double) distArray[i];
			_return.push_back(neighbor);
		}
	}
	void buildCategory(const std::string& categoryName) {
		std::cout << "RPC buildCategory" << std::endl;
		ANNTreeHelper::instance()->buildCategory(categoryName);
	}

	void buildAllCategory() {
		std::cout << "RPC buildAllCategory" << std::endl;
		ANNTreeHelper::instance()->buildAllCategory();
	}

	void query(std::vector<std::string> & _return, const std::string& imagePath,
			const int32_t treeIndex, const std::string& featureType,
			const int32_t k) {
		std::cout << "RPC query" << std::endl;
		if (featureType != "color" && featureType != "shape"
				&& featureType != "surf") {
			return;
		}
		boost::shared_ptr<Feature> pFeature;
		if (featureType == "color") {
			pFeature.reset(
					new ColorFeature(GlobalConfig::COLOR_FEATURE_BINSIZE));
		}
		if (featureType == "shape") {
			pFeature.reset(
					new ShapeFeature(GlobalConfig::SHAPE_FEATURE_BINSIZE,
							GlobalConfig::SHAPE_FEATURE_BINSIZE));
		}
		if (featureType == "surf") {
			pFeature.reset(
					new SURFFeature(
							GlobalConfig::SURF_FEATURE_COUNT_PER_IMAGE));
		}
		cv::Mat image = cv::imread(imagePath);
		if (image.empty()) {
			return;
		}
		pFeature->compute(image);
		boost::shared_ptr<ANNTreeRoot> pRoot;
		ANNTreeRootPool::instance()->get(&pRoot, treeIndex);
		std::vector<Neighbor> neighborArray;
		pFeature->knnSearch(&neighborArray, pRoot, k);
		std::cout << "treeIndex " << treeIndex << " neighborArray size: "
				<< neighborArray.size() << std::endl;
		_return.reserve(neighborArray.size());
		for (size_t i = 0; i < neighborArray.size(); ++i) {
			_return.push_back(
					boost::lexical_cast<std::string>(neighborArray[i].id));
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

