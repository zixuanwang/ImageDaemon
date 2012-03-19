// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <server/TNonblockingServer.h>
#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include <unistd.h>
#include <boost/shared_ptr.hpp>
#include "ANNTreeDaemon.h"
#include "ANNTreeRoot.h"
#include "ANNTreeSlave.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

using namespace ::ANNTreeDaemon;

class ANNTreeDaemonHandler: virtual public ANNTreeDaemonIf {
public:
	ANNTreeDaemonHandler(const std::string& confPath, bool build, bool slave,
			int slavePort) {
		if (build) {
			ANNTreeRoot::instance()->init(confPath, slavePort);
//			ANNTreeRoot::instance()->buildFeature("face",
//					"/home/zixuan/public_html/face/feature", "face/image",
//					"/home/zixuan/public_html/face/list.txt");
			ANNTreeRoot::instance()->buildFeature("bag",
					"/home/zixuan/public_html/shoe/feature", "shoe/image",
					"/home/zixuan/public_html/shoe/list.txt");
		}
		if (slave) {
			ANNTreeSlave::instance()->init();
		}

	}

	void knnSearch(std::vector<Neighbor> & _return, const std::string& feature,
			const int32_t k) {
		ANNTreeRoot::instance()->knnSearch(&_return, feature, k);
	}

	void addFeature(const int64_t id, const std::string& feature) {
		ANNTreeRoot::instance()->addFeature(id, feature);
	}

	void build() {

	}

	void slaveIndex() {
		std::cout << "RPC slaveIndex" << std::endl;
		ANNTreeSlave::instance()->index();
	}

	void slaveInit() {
		ANNTreeSlave::instance()->init();
	}

	void slaveSave() {

	}

	void slaveAddFeature(const int64_t id, const std::string& feature) {
		std::cout << "RPC slaveAddFeature" << std::endl;
		std::vector<float> fFeature;
		TypeConverter::readStringToArray(&fFeature, feature);
		ANNTreeSlave::instance()->addFeature(id, fFeature);
	}

	void slaveKnnSearch(std::vector<Neighbor> & _return,
			const std::string& feature, const int32_t k) {
		std::vector<int64_t> neighborIdArray;
		std::vector<float> distArray;
		std::vector<float> fFeature;
		TypeConverter::readStringToArray(&fFeature, feature);
		ANNTreeSlave::instance()->knnSearch(&neighborIdArray, &distArray,
				fFeature, k);
		for (size_t i = 0; i < neighborIdArray.size(); ++i) {
			Neighbor neighbor;
			neighbor.id = neighborIdArray[i];
			neighbor.distance = (double) distArray[i];
			_return.push_back(neighbor);
		}
	}
	void similarSearch(std::vector<Neighbor> & _return, const int64_t id,
			const int32_t k, const std::string& type) {
		std::cout << "RPC similarSearch" << std::endl;
		ANNTreeRoot::instance()->similarSearch(&_return, id, k, type);
	}

};

int main(int argc, char *argv[]) {
	if (argc == 1) {
		std::cout << "Usage: ANNTreeDaemon -c <confPath> -b -p 9999"
				<< std::endl;
		return 0;
	}
	int port = 9999;
	std::string confPath;
	bool build = false;
	bool slave = false;
	int c;
	while ((c = getopt(argc, argv, "c:bsp:")) != -1) {
		switch (c) {
		case 'c':
			confPath = strdup(optarg);
			break;
		case 'b':
			build = true;
			break;
		case 's':
			slave = true;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		}
	}
	shared_ptr<ANNTreeDaemonHandler> handler(
			new ANNTreeDaemonHandler(confPath, build, slave, port - 1));
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

