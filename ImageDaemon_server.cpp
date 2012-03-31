#include "GlobalConfig.h"
#include "ImageDaemon.h"
#include "LocalFeatureExtractor.h"
#include "ANNVocabulary.h"
#include "TypeConverter.h"
#include "Image.h"
#include "LocalFeature.h"
#include "BoWHistogram.h"
#include "ColorFeature.h"
#include "ShapeFeature.h"
#include "SURFFeature.h"
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
using namespace ::net::walnutvision;

class ImageDaemonHandler: virtual public ImageDaemonIf {
public:
	ImageDaemonHandler() {
		ANNVocabulary::instance()->init("/export/c/voc/surf.1m.voc.dat");
		Logger::instance()->log("Initialization Done.");
	}
	void computeBoWFeature(std::vector<Bin> & _return, const int64_t rowKey) {
		BoWHistogram histogram;
		Image image;
		image.load(rowKey);
		//compute the feature
		LocalFeatureExtractor localFeatureExtractor;
		LocalFeature localFeature;
		localFeatureExtractor.extractFeature(image.image, localFeature.keypoint,
				localFeature.descriptor);
		localFeature.save(rowKey);
		//quantization
		ANNVocabulary::instance()->quantizeFeature(localFeature, &histogram);
		histogram.save(rowKey);
		histogram.convert(&_return);
		Logger::instance()->log("RPC computeBoWFeature");
	}

	void computeColorFeature(const int64_t rowKey) {
		boost::shared_ptr<Feature> pFeature(
				new ColorFeature(GlobalConfig::COLOR_FEATURE_BINSIZE));
		Image image;
		image.load(rowKey);
		pFeature->compute(image.image);
		std::string string;
		pFeature->save(&string);
		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
		dbAdapter->saveCell(string, GlobalConfig::IMAGE_TABLE, rowKey,
				GlobalConfig::IMAGE_COLOR_FEATURE_COLUMN);
		Logger::instance()->log("RPC computeColorFeature");
	}

	void computeShapeFeature(const int64_t rowKey) {
		boost::shared_ptr<Feature> pFeature(
				new ShapeFeature(GlobalConfig::SHAPE_FEATURE_BINSIZE,
						GlobalConfig::SHAPE_FEATURE_BINSIZE));
		Image image;
		image.load(rowKey);
		pFeature->compute(image.image);
		std::string string;
		pFeature->save(&string);
		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
		dbAdapter->saveCell(string, GlobalConfig::IMAGE_TABLE, rowKey,
				GlobalConfig::IMAGE_SHAPE_FEATURE_COLUMN);
		Logger::instance()->log("RPC computeShapeFeature");
	}

	void computeSURFFeature(const int64_t rowKey) {
		boost::shared_ptr<Feature> pFeature(new SURFFeature(GlobalConfig::SURF_FEATURE_COUNT_PER_IMAGE));
		Image image;
		image.load(rowKey);
		pFeature->compute(image.image);
		std::string string;
		pFeature->save(&string);
		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
		dbAdapter->saveCell(string, GlobalConfig::IMAGE_TABLE, rowKey,
				GlobalConfig::IMAGE_SURF_FEATURE_COLUMN);
		Logger::instance()->log("RPC computeSURFFeature");
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

	/// This function is called for real time indexing
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

