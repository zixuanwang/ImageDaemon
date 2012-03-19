/*
 * test.h
 *
 *  Created on: Jan 11, 2012
 *      Author: zixuan
 */

#ifndef TEST_H_
#define TEST_H_
#include "GlobalConfig.h"
#include "Image.h"
#include "LocalFeature.h"
#include "LocalFeatureExtractor.h"
#include "ImageDaemon_types.h"
#include "RankItem.h"
#include "Hbase.h"
#include "Ticker.h"
#include "ImageResizer.h"
#include "ShapeFeature.h"
#include "ColorFeature.h"
using namespace ::ImageDaemon;
void testFeature() {
	int64_t rowKey = 0;
	Image image;
	image.load(rowKey);
	LocalFeatureExtractor localFeatureExtractor;
	LocalFeature localFeature;
	localFeatureExtractor.extractFeature(image.image, localFeature.keypoint,
			localFeature.descriptor);
	localFeature.save(rowKey);
	LocalFeature localFeature2;
	localFeature2.load(rowKey);
	for (size_t i = 0; i < localFeature2.keypoint.size(); ++i) {
		cv::circle(image.image, localFeature2.keypoint[i].pt,
				localFeature2.keypoint[i].size, CV_RGB(255,0,0), 1, CV_AA, 0);
	}
	cv::namedWindow("test");
	cv::imshow("test", image.image);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void testHistogram() {
	int64_t rowKey = 0;
	Image image;
	image.load(rowKey);
	LocalFeature localFeature;
	LocalFeatureExtractor localFeatureExtractor;
	localFeatureExtractor.extractFeature(image.image, localFeature.keypoint,
			localFeature.descriptor);
	localFeature.save(rowKey);
	BoWHistogram histogram;
	ANNVocabulary::instance()->quantizeFeature(localFeature, &histogram);
	std::vector<Bin> _return;
	histogram.convert(&_return);
	for (size_t i = 0; i < _return.size(); ++i) {
		std::cout << _return[i].visualwordID << "\t" << _return[i].frequency
				<< std::endl;
	}
}

void testResizer() {
	std::string imagePath = "/home/zixuan/Desktop/Firefox_wallpaper.png";
	std::string cropImagePath = "/home/zixuan/Desktop/crop.jpg";
	ImageResizer::crop(imagePath, cropImagePath, 160, 160);
}

void testHbase() {
	Ticker t;
	t.start();
	boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	HbaseClient client(protocol);
	transport->open();
	transport->close();
	t.stop();
}
struct PostingComparator {
	bool operator()(const Posting &i, const Posting &j) {
		return (i.score < j.score);
	}
};

//void testShape() {
//	std::string imageDirectory = "/home/zixuan/public_html/shoe/image";
//	std::string shapeOutput = "/home/zixuan/public_html/shoe/feature/shape.txt";
//	std::string colorOutput = "/home/zixuan/public_html/shoe/feature/color.txt";
//	std::string list = "/home/zixuan/public_html/shoe/list.txt";
//	std::ofstream outStream;
//	outStream.open(shapeOutput.c_str());
//	std::ofstream outStream2;
//	outStream2.open(list.c_str());
//	std::ofstream outStream3;
//	outStream3.open(colorOutput.c_str());
//	boost::filesystem::directory_iterator end_itr;
//	for (boost::filesystem::directory_iterator itr(imageDirectory);
//			itr != end_itr; ++itr) {
//		if (!boost::filesystem::is_directory(itr->status())) {
//			boost::shared_ptr<Feature> pFeature(new ShapeFeature(20, 20));
//			boost::shared_ptr<Feature> pColor(new ColorFeature(10));
//			std::string filePath = itr->path().string();
//			cv::Mat image = cv::imread(filePath);
//			cv::Mat colorMask;
//			cv::Mat shapeMask;
//			pFeature->segment(shapeMask, image);
////			pColor->segment(colorMask, image);
//			cv::Mat res;
//			image.copyTo(res, shapeMask);
//			pFeature->compute(image, shapeMask);
//			pColor->compute(image, colorMask);
//			const std::vector<float>& feature = pFeature->getFeature();
//			size_t i = 0;
//			for (; i < feature.size() - 1; ++i) {
//				outStream << feature[i] << ",";
//			}
//			outStream << feature[i] << std::endl;
//			std::vector<float> feature2 = pColor->getFeature();
//			i = 0;
//			for (; i < feature2.size() - 1; ++i) {
//				outStream3 << feature2[i] << ",";
//			}
//			outStream3 << feature2[i] << std::endl;
//			boost::filesystem::path bstPath = filePath;
//			outStream2 << bstPath.filename().string() << std::endl;
//			std::cout << "computing feature for " << filePath << std::endl;
//			cv::imwrite(
//					"/home/zixuan/Desktop/tmp/" + bstPath.filename().string(),
//					res);
//		}
//	}
//}

#endif /* TEST_H_ */
