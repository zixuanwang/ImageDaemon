#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <server/TThreadPoolServer.h>
#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>

#include "GlobalConfig.h"

#include "ImageDaemon.h"
#include "LocalFeatureExtractor.h"
#include "ANNVocabulary.h"
#include "TypeConverter.h"
#include "Image.h"
#include "LocalFeature.h"
#include "InvertedIndex.h"
#include "BoWHistogram.h"
#include "ColorFeature.h"
#include "test.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

using namespace ::ImageDaemon;

class ImageDaemonHandler: virtual public ImageDaemonIf {
public:
	ImageDaemonHandler() {
//		HbaseAdapter::instance()->init();
		MongoDBAdapter::instance()->init();
		ANNVocabulary::instance()->init("/media/data/voc/surf.1m.voc.dat");
		std::cout << "Init Done." << std::endl;
		//testQuery();
	}
	void getBoWFeature(std::vector<Bin> & _return, const int64_t rowKey) {
		// called by the mapper and the querier
		// image is loaded from Hbase
		if (MongoDBAdapter::instance()->checkExist(GlobalConfig::IMAGE_TABLE,
				rowKey, GlobalConfig::IMAGE_BOW_HISTOGRAM_COLUMN)) {
			BoWHistogram histogram;
			histogram.load(rowKey);
			histogram.convert(&_return);
//			std::cout << "Loading " << rowKey << std::endl;
		} else {
			Image image;
			image.load(rowKey);
			//compute the feature
			LocalFeatureExtractor localFeatureExtractor;
			LocalFeature localFeature;
			localFeatureExtractor.extractFeature(image.image,
					localFeature.keypoint, localFeature.descriptor);
			localFeature.save(rowKey);
			//quantization
			BoWHistogram histogram;
			ANNVocabulary::instance()->quantizeFeature(localFeature,
					&histogram);
			histogram.save(rowKey);
			histogram.convert(&_return);
			std::cout << "Computing " << rowKey << "\t" << histogram.size()
					<< " features" << std::endl;
		}
	}

	void addPostingList(const int64_t visualwordID,
			const std::vector<Posting> & postingArray) {
		// called by the reducer
		try {
			InvertedIndex::instance()->savePostingList(visualwordID,
					postingArray);
//			std::cout << "Adding " << visualwordID << std::endl;
		} catch (...) {
			std::cout << "Default error handler" << std::endl;
		}
	}

	void addImage(const std::string& imageHash, const int64_t rowKey) {
		MongoDBAdapter::instance()->saveCell(imageHash,
				GlobalConfig::IMAGE_TABLE, rowKey,
				GlobalConfig::IMAGE_HASH_COLUMN);
	}
	void computeColorFeature(const int64_t rowKey) {
		Image image;
		image.load(rowKey);
		ColorFeature colorFeature;
		colorFeature.compute(image.image);
		std::string string;
		colorFeature.save(&string);
		MongoDBAdapter::instance()->saveCell(string, GlobalConfig::IMAGE_TABLE,
				rowKey, GlobalConfig::IMAGE_COLOR_FEATURE_COLUMN);
	}
	void query(std::vector<std::string> & _return,
			const std::string& imagePath) {
		_return.clear();
		cv::Mat image = cv::imread(imagePath, 0);
		LocalFeature localFeature;
		LocalFeatureExtractor localFeatureExtractor;
		localFeatureExtractor.extractFeature(image, localFeature.keypoint,
				localFeature.descriptor);
		BoWHistogram histogram;
		ANNVocabulary::instance()->quantizeFeature(localFeature, &histogram);
		const std::vector<int64_t>& visualwordIdArray =
				histogram.visualwordIdArray();
		std::cout << "histogram size: " << histogram.size() << std::endl;
		// TODO: modify the query pipeline
		boost::unordered_map<int64_t, double> candidateScoreMap;
		for (size_t i = 0; i < visualwordIdArray.size(); ++i) {
			std::vector<int64_t> imageIdArray;
			std::vector<double> imageScoreArray;
			InvertedIndex::instance()->loadPostingList(&imageIdArray,
					&imageScoreArray, visualwordIdArray[i]);
			for (size_t j = 0;
					j < imageIdArray.size() && j < imageScoreArray.size();
					++j) {
				candidateScoreMap[imageIdArray[j]] += imageScoreArray[j];
			}
		}
		std::vector<RankItem<int64_t, double> > candidateRankList;
		candidateRankList.reserve(candidateScoreMap.size());
		for (boost::unordered_map<int64_t, double>::iterator iter =
				candidateScoreMap.begin(); iter != candidateScoreMap.end();
				++iter) {
			RankItem<int64_t, double> item(iter->first, iter->second);
			candidateRankList.push_back(item);
		}
		size_t candidateSize = 50;
		if (candidateRankList.size() > candidateSize) {
			std::nth_element(candidateRankList.begin(),
					candidateRankList.begin() + candidateSize,
					candidateRankList.end());
		}
		std::vector<double> queryVector;
		histogram.flatten(&queryVector, ANNVocabulary::instance()->size());
		std::vector<RankItem<int64_t, double> > rankList;
		for (size_t i = 0; i < candidateRankList.size() && i < candidateSize;
				++i) {
			int64_t otherImageId = candidateRankList[i].index;
			BoWHistogram otherHistogram;
			otherHistogram.load(otherImageId);
			double score = otherHistogram.innerProduct(queryVector);
			RankItem<int64_t, double> item(otherImageId, -1.0 * score);
			rankList.push_back(item);
		}
		std::sort(rankList.begin(), rankList.end());
		for (int i = 0; i < 10 && i < (int) rankList.size(); ++i) {
			std::string path;
			MongoDBAdapter::instance()->loadCell(&path,
					GlobalConfig::IMAGE_TABLE, rankList[i].index,
					GlobalConfig::IMAGE_HASH_COLUMN);
			_return.push_back(path);
		}
	}
};

int main(int argc, char **argv) {
	int port = 9992;
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

