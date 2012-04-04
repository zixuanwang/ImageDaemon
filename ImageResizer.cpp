/*
 * ImageResizer.cpp
 *
 *  Created on: Mar 10, 2012
 *      Author: zixuan
 */

#include "ImageResizer.h"

ImageResizer::ImageResizer() {

}

ImageResizer::~ImageResizer() {

}

void ImageResizer::crop(const std::string& imagePath,
		const std::string& cropImagePath, int width, int height) {
	cv::Mat image = cv::imread(imagePath);
	cv::Mat cropImage(160, 160, CV_8UC3, cv::Scalar(255, 255, 255));
	try {
		if (image.empty() || width < 0 || height < 0) {
			cv::imwrite(cropImagePath, cropImage);
			return;
		}
		if (!image.empty() && width == 0 && height > 0) {
			double ratio = (double) height / image.rows;
			cv::resize(image, cropImage, cv::Size(), ratio, ratio,
					cv::INTER_LINEAR);
		}
		if (!image.empty() && height == 0 && width > 0) {
			double ratio = (double) width / image.cols;
			cv::resize(image, cropImage, cv::Size(), ratio, ratio,
					cv::INTER_LINEAR);
		}
		if (!image.empty() && width > 0 && height > 0) {
			int rows = image.rows;
			int cols = image.cols;
			double widthRatio = (double) width / cols;
			double heightRatio = (double) height / rows;
			double ratio = widthRatio > heightRatio ? widthRatio : heightRatio;
			cv::Mat resizedImage;
			cv::resize(image, resizedImage, cv::Size(), ratio, ratio,
					cv::INTER_LINEAR);
			int centerX = resizedImage.cols / 2;
			int centerY = resizedImage.rows / 2;
			cropImage = resizedImage(
					cv::Range(centerY - height / 2, centerY + (height + 1) / 2),
					cv::Range(centerX - width / 2, centerX + (width + 1) / 2));
		}
	} catch (cv::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
	cv::imwrite(cropImagePath, cropImage);
}

void ImageResizer::resize(const cv::Mat& src, cv::Mat* pDst, int maxLength) {
	*pDst = cv::Mat();
	if (src.empty()) {
		return;
	}
	int rows = src.rows;
	int cols = src.cols;
	int length = rows > cols ? rows : cols;
	double ratio = (double) maxLength / length;
	try {
		cv::resize(src, *pDst, cv::Size(), ratio, ratio, cv::INTER_LINEAR);
	} catch (cv::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
