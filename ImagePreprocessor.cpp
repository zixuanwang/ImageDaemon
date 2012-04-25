/*
 * ImagePreprocessor.cpp
 *
 *  Created on: Apr 16, 2012
 *      Author: zixuan
 */

#include "GlobalConfig.h"
#include "HbaseAdapter.h"
#include "ImageDaemon.h"
#include "TypeConverter.h"
#include "ShapeFeature.h"
#include "ColorFeature.h"
#include "Image.h"
#include "File.h"
#include "ScopedImageDaemonClient.h"

std::vector<std::string> sSlaveNameArray;
std::vector<std::string> sRowKeyArray;
std::vector<int64_t> sImageKeyArray;

class ApplyPreprocessor {
public:
	ApplyPreprocessor(std::vector<std::string>* _pRowKeyArray,
			std::vector<int64_t>* _pImageKeyArray, int slaveCount) :
			pRowKeyArray(_pRowKeyArray), pImageKeyArray(_pImageKeyArray), mSlaveCount(
					slaveCount) {
	}

	~ApplyPreprocessor() {

	}

	void operator()(const tbb::blocked_range<size_t>& r) const {
		for (size_t i = r.begin(); i != r.end(); ++i) {
			try {
				int index = rand() % mSlaveCount;
				ScopedImageDaemonClient client(sSlaveNameArray[index],
						GlobalConfig::IMAGE_DAEMON_SERVER_PORT);
				client.client->addImage(pRowKeyArray->at(i),
						pImageKeyArray->at(i));
				client.client->cropImage(pImageKeyArray->at(i), 160, 160);
				client.client->computeColorFeature(pImageKeyArray->at(i));
				client.client->computeShapeFeature(pImageKeyArray->at(i));
				client.client->computeSURFFeature(pImageKeyArray->at(i));
				std::cout << "rowKey: " << pRowKeyArray->at(i) << "\t"
						<< "imageKey: " << pImageKeyArray->at(i) << std::endl;
				// move data
//				boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
//				dbAdapter->copyRow("test_image", "image", pRowKeyArray->at(i));
//				dbAdapter->deleteRow("test_image", pRowKeyArray->at(i));
			} catch (const TException& tx) {
				std::cerr << tx.what() << std::endl;
			}
		}
	}
private:
	std::vector<std::string>* pRowKeyArray;
	std::vector<int64_t>* pImageKeyArray;
	int mSlaveCount;

};

void initialize(const std::string& confPath) {
	// load configuration
	std::ifstream inStream;
	inStream.open(confPath.c_str());
	if (inStream.good()) {
		std::string server;
		while (getline(inStream, server)) {
			sSlaveNameArray.push_back(server);
		}
		inStream.close();
	}
	if (sSlaveNameArray.empty()) {
		std::cerr << "Error in loading configuration file in ANNTreeRoot"
				<< std::endl;
		return;
	}
	std::cout << "Initialization done. There are " << sSlaveNameArray.size()
			<< " slaves." << std::endl;
}

void getImageTable() {
	sRowKeyArray.clear();
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter());
	std::vector<std::string> columnNameArray;
	columnNameArray.push_back("d:id_0");
	int scanner = dbAdapter->scannerOpen("image", "", columnNameArray);
	while (true) {
		std::string strRowKey;
		std::map<std::string, std::string> rowMap;
		dbAdapter->scannerGet(&strRowKey, &rowMap, scanner);
		if (rowMap.empty()) {
			break;
		}
		sRowKeyArray.push_back(strRowKey);
		std::string strImageKey = rowMap["d:id_0"];
		int64_t imageKey;
		TypeConverter::readStringToNum(&imageKey, strImageKey);
		sImageKeyArray.push_back(imageKey);
	}
	dbAdapter->scannerClose(scanner);
}

void run() {
	if (sRowKeyArray.size() != sImageKeyArray.size()) {
		return;
	}
	tbb::parallel_for(
			tbb::blocked_range<size_t>(0, sRowKeyArray.size()),
			ApplyPreprocessor(&sRowKeyArray, &sImageKeyArray,
					sSlaveNameArray.size()));
}

//void outputCluster(const std::vector<std::string>& samplePathArray,
//		const cv::Mat& cluster, int k) {
//	boost::filesystem::path bstOutputPath = "/home/zixuan/Desktop/ouput";
//	bstOutputPath /= boost::lexical_cast<std::string>(k);
//	boost::filesystem::create_directories(bstOutputPath);
//	for (int i = 0; i < cluster.rows; ++i) {
//		const int* ptr = cluster.ptr<int>(i);
//		int index = *ptr;
//		std::string dir = (bstOutputPath
//				/ boost::lexical_cast<std::string>(index)).string();
//		boost::filesystem::create_directories(dir);
//		boost::filesystem::path srcPath = samplePathArray[i];
//		std::string fileName = srcPath.filename().string();
//		std::string cmd = "cp ";
//		cmd += samplePathArray[i] + " " + dir + "/" + fileName;
//		system(cmd.c_str());
//		std::cout << cmd << std::endl;
//	}
//}
//
//void testCluster() {
//	std::string inputDirectory = "/home/zixuan/Downloads/shoes/imgsNew";
//	std::vector<std::string> imagePathArray;
//	File::getFiles(&imagePathArray, inputDirectory);
//
//	std::vector<float> sampleArray; // container to hold the data
//	int sampleCount = 0;
//	std::vector<std::string> samplePathArray;
//	for (size_t i = 0; i < imagePathArray.size(); ++i) {
//		boost::shared_ptr<Feature> pFeature(
//				new ColorFeature(GlobalConfig::COLOR_FEATURE_BINSIZE));
//		cv::Mat image = cv::imread(imagePathArray[i]);
//		pFeature->compute(image);
//		if (!pFeature->empty()) {
//			samplePathArray.push_back(imagePathArray[i]);
//			std::copy(pFeature->getFeature().begin(),
//					pFeature->getFeature().end(),
//					std::back_inserter(sampleArray));
//			++sampleCount;
//		}
//		std::cout << "Processing " << imagePathArray[i] << std::endl;
//	}
//	cv::Mat sampleMat(sampleArray, false);
//	sampleMat = sampleMat.reshape(0, sampleCount);
//	for (int k = 10; k <= 100; k += 10) {
//		cv::Mat clusters = cv::Mat(sampleMat.rows, 1, CV_32SC1);
//		cv::kmeans(sampleMat, k, clusters,
//				cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 100, 1e-6),
//				5, cv::KMEANS_PP_CENTERS);
//		outputCluster(samplePathArray, clusters, k);
//	}
//}

//void test() {
//	std::string outputDirectory = "/export/walnut/Desktop/tmp/";
//	std::vector<int64_t> imageKeyArray;
//	std::vector<std::string> imageHashArray;
//	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter());
//	std::vector<std::string> columnNameArray;
//	columnNameArray.push_back(GlobalConfig::IMAGE_HASH_COLUMN);
//	int scanner = dbAdapter->scannerOpen(GlobalConfig::IMAGE_TABLE, "",
//			columnNameArray);
//	while (true) {
//		std::string strRowKey;
//		std::map<std::string, std::string> rowMap;
//		dbAdapter->scannerGet(&strRowKey, &rowMap, scanner);
//		if (rowMap.empty()) {
//			break;
//		}
//		int64_t imageKey;
//		TypeConverter::readStringToNum(&imageKey, strRowKey);
//		imageKeyArray.push_back(imageKey);
//		std::string imageHash = rowMap[GlobalConfig::IMAGE_HASH_COLUMN];
//		imageHashArray.push_back(imageHash);
//		std::cout << imageKey << "\t" << imageHash << std::endl;
//	}
//	dbAdapter->scannerClose(scanner);
//	for (size_t i = 0; i < imageKeyArray.size(); ++i) {
//		Image image;
//		image.load(imageKeyArray[i], GlobalConfig::IMAGE_MASK_DATA);
//		cv::imwrite(outputDirectory + imageHashArray[i] + ".jpg", image.image);
//	}
//}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "Usage: ImagePreprocessor -c conf.txt" << std::endl;
		return 0;
	}
	std::string confPath;
	int c;
	while ((c = getopt(argc, argv, "c:")) != -1) {
		switch (c) {
		case 'c':
			confPath = strdup(optarg);
			break;
		}
	}
	tbb::task_scheduler_init init(16);
	initialize(confPath);
	getImageTable();
	run();
//	testCluster();
}
