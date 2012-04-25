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
#include "SURFFeature.h"
#include "ScopedANNTreeDaemonClient.h"
#include "Serializer.h"
using namespace ::net::walnutvision;

void testRemoveHead() {
	std::string fileName =
			"/media/node1/export/walnut/workspace/category/yt.map";
	std::ifstream inStream;
	inStream.open(fileName.c_str());
	std::ofstream outStream("/media/node1/export/walnut/workspace/category/yt");
	if (inStream.good()) {
		std::string line;
		while (getline(inStream, line)) {
			std::string newline = line.substr(1);
			outStream << newline << std::endl;
		}
		inStream.close();
	}
	outStream.close();
}

//void testSerializer() {
//	std::string imagePath = "/home/zixuan/Desktop/test.jpg";
//	std::string path = "/home/zixuan/Desktop/image.dat";
//	cv::Mat image = cv::imread(imagePath);
//	Serializer::save(image, path);
//	cv::Mat image2;
//	Serializer::load(&image2, path);
//	cv::imwrite("/home/zixuan/Desktop/test2.jpg", image2);
//}

//void testClient(){
//	ScopedANNTreeDaemonClient client("",8080);
//	try{
//		client.client->buildAllCategory();
//	}catch (const TException& tx) {
//		std::cerr << tx.what() << std::endl;
//	}
//}

//void testSegment() {
//	cv::Mat image = cv::Mat();
//	cv::imwrite("/home/zixuan/Desktop/result.jpg", image);
//	cv::Mat image2 = cv::imread("/home/zixuan/Desktop/result.jpg");
//	if (image2.empty()) {
//		std::cout << "empty" << std::endl;
//	}
//	std::string imageDirectory = "/home/zixuan/public_html/bags/imgs";
//	std::string outputDirctory = "/home/zixuan/Desktop/bags";
//	boost::filesystem::directory_iterator end_itr;
//	for (boost::filesystem::directory_iterator itr(imageDirectory);
//			itr != end_itr; ++itr) {
//		if (!boost::filesystem::is_directory(itr->status())) {
//			std::string imagePath = itr->path().string();
//			boost::filesystem::path bstoutputdir = outputDirctory;
//			std::string maskPath =
//					(bstoutputdir / itr->path().filename()).string();
//			cv::Mat image = cv::imread(imagePath);
//			cv::Mat mask;
//			ColorFeature feature(10);
//			feature.segment(&mask, image);
//			mask *= 255;
//			cv::imwrite(maskPath, mask);
//			std::cout << imagePath << std::endl;
//		}
//	}
//}

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
