#include "InvertedIndexDaemon.h"
#include "GlobalConfig.h"
#include "InvertedIndex.h"
#include "Logger.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

using namespace ::InvertedIndexDaemon;

class InvertedIndexDaemonHandler: virtual public InvertedIndexDaemonIf {
public:
	InvertedIndexDaemonHandler() {
		InvertedIndex::instance()->init();
		Logger::instance()->log("Initialization done");
	}

	void init() {
		InvertedIndex::instance()->init();
		Logger::instance()->log("RPC init");
	}

	void savePostingList(const int64_t visualwordId,
			const std::vector<Posting> & postingArray) {
		InvertedIndex::instance()->savePostingList(visualwordId, postingArray);
		Logger::instance()->log("RPC savePostingList");
	}

	void loadPostingList(std::vector<Posting> & _return,
			const int64_t visualwordId) {
		InvertedIndex::instance()->loadPostingList(&_return, visualwordId);
		Logger::instance()->log("RPC loadPostingList");
	}

	void addPosting(const int64_t visualwordId, const Posting& posting) {
		InvertedIndex::instance()->addPosting(visualwordId, posting);
		Logger::instance()->log("RPC addPosting");
	}

};

int main(int argc, char **argv) {
	int port = 9991;
	shared_ptr<InvertedIndexDaemonHandler> handler(
			new InvertedIndexDaemonHandler());
	shared_ptr<TProcessor> processor(new InvertedIndexDaemonProcessor(handler));
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

