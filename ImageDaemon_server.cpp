#include "GlobalConfig.h"
#include "ImageDaemon.h"
#include "LocalFeatureExtractor.h"
#include "ANNVocabulary.h"
#include "TypeConverter.h"
#include "Image.h"
#include "LocalFeature.h"
#include "BoWHistogram.h"
#include "ColorFeature.h"
#include "ImageRecognizer.h"
#include "InvertedIndexClient.h"
#include "Ticker.h"
#include "Logger.h"
#include "ImageResizer.h"
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
		ANNVocabulary::instance()->init("/export/c/voc/surf.1m.voc.dat");
		Logger::instance()->log("Initialization Done.");
	}
	void getBoWFeature(std::vector<Bin> & _return, const int64_t rowKey) {
		// called by the mapper and the querier
		BoWHistogram histogram;
		histogram.load(rowKey);
		if (!histogram.empty()) {
			histogram.convert(&_return);
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
		}
		Logger::instance()->log("RPC getBoWFeature");
	}

	void computeColorFeature(const int64_t rowKey) {
		Image image;
		image.load(rowKey);
		boost::shared_ptr<Feature> pFeature(new ColorFeature);
		cv::Mat mask;
		pFeature->segment(&mask, image.image);
		pFeature->compute(image.image, mask);
		std::string string;
		pFeature->save(&string);
		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
		dbAdapter->saveCell(string, GlobalConfig::IMAGE_TABLE, rowKey,
				GlobalConfig::IMAGE_COLOR_FEATURE_COLUMN);
		Logger::instance()->log("RPC computeColorFeature");
	}

	void query(std::vector<std::string> & _return,
			const std::string& imagePath) {
		ImageRecognizer imageRecognizer;
		imageRecognizer.queryBoWFeature(&_return, imagePath);
		Logger::instance()->log("RPC query");
	}

	void addImage(const std::string& imageHash, const int64_t rowKey) {
		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
		std::string strRowKey;
		TypeConverter::readNumToString(&strRowKey, rowKey);
		dbAdapter->saveCell(imageHash, GlobalConfig::IMAGE_TABLE, strRowKey,
				GlobalConfig::IMAGE_HASH_COLUMN);
		Logger::instance()->log("RPC addImage");
	}

	void indexImage(const std::string& imageHash, const int64_t rowKey) {
		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
		std::string strRowKey;
		TypeConverter::readNumToString(&strRowKey, rowKey);
		dbAdapter->saveCell(imageHash, GlobalConfig::IMAGE_TABLE, strRowKey,
				GlobalConfig::IMAGE_HASH_COLUMN);
		Image image;
		image.load(imageHash);
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
		const std::vector<int64_t>& visualwordIdArray =
				histogram.visualwordIdArray();
		const std::vector<double>& frequencyArray = histogram.frequencyArray();
		for (size_t i = 0; i < visualwordIdArray.size(); ++i) {
			int64_t visualwordId = visualwordIdArray[i];
			Posting posting;
			posting.imageId = rowKey;
			posting.score = -1.0 * frequencyArray[i];
			InvertedIndexClient invertedIndexClient;
			invertedIndexClient.addPosting(visualwordId, posting);
		}
		Logger::instance()->log("RPC indexImage");
	}

	void cropImage(const std::string& imagePath,
			const std::string& cropImagePath, const int32_t width,
			const int32_t height) {
		ImageResizer::crop(imagePath, cropImagePath, width, height);
	}
};

int main(int argc, char **argv) {
	int port = 9992;
	shared_ptr<ImageDaemonHandler> handler(new ImageDaemonHandler());
	shared_ptr<TProcessor> processor(new ImageDaemonProcessor(handler));
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

