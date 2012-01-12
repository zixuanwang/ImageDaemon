#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <server/TThreadPoolServer.h>
#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>

#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "ImageDaemon.h"
#include "LocalFeatureExtractor.h"
#include "ANNVocabulary.h"
#include "TypeConverter.h"
#include "Image.h"
#include "LocalFeature.h"
#include "InvertedIndex.h"
#include "BoWHistogram.h"
#include "test.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

using namespace ::ImageDaemon;

#define CONTENT_TABLE "contentTable"
#define INDEX_TABLE "indexTable"

class ImageDaemonHandler: virtual public ImageDaemonIf {
public:
	ImageDaemonHandler() {
		HbaseAdapter::instance()->init();
		ANNVocabulary::instance()->init(
				"/home/zixuan/data/vocabulary/oxford.surf.200k.voc.dat");
		std::cout << "Init Done." << std::endl;
		testQuery();

	}

	void getBoWFeatureFromHbase(std::vector<Bin> & _return,
			const int64_t rowKey) {
		// called by the mapper and the querier
		Image image;
		image.loadImageData(rowKey);
		//compute the feature
		LocalFeatureExtractor localFeatureExtractor;
		LocalFeature localFeature;
		localFeatureExtractor.extractFeature(image.image, localFeature.keypoint,
				localFeature.descriptor);
		localFeature.save(rowKey);
		//quantization
		BoWHistogram histogram;
		ANNVocabulary::instance()->quantizeFeature(localFeature, &histogram);
		histogram.save(rowKey);
		histogram.convert(&_return);
		std::cout << "Computing " << rowKey << std::endl;
	}

	void addPostingList(const int64_t visualwordID,
			const std::vector<Posting> & postingArray) {
		// called by the reducer
		InvertedIndex::instance()->savePostingList(visualwordID, postingArray);
		std::cout << "Adding " << visualwordID << std::endl;
	}
};

int main(int argc, char **argv) {
	int port = 9091;
	shared_ptr<ImageDaemonHandler> handler(new ImageDaemonHandler());
	shared_ptr<TProcessor> processor(new ImageDaemonProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(
			new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
	shared_ptr<ThreadManager> threadManager =
			ThreadManager::newSimpleThreadManager(200);
	shared_ptr<PosixThreadFactory> threadFactory =
			shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
	threadManager->threadFactory(threadFactory);
	threadManager->start();
	TThreadPoolServer server(processor, serverTransport, transportFactory,
			protocolFactory, threadManager);
	server.serve();
	return 0;
}

