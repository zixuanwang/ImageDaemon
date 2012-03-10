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
	if (image.data == NULL || width <= 0 || height <= 0) {
		// empty image
		cv::Mat cropImage(width, height, CV_8UC3, cv::Scalar(255, 255, 255));
		cv::imwrite(cropImagePath, cropImage);
	} else {
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
		cv::Mat cropImage = resizedImage(
				cv::Range(centerY - height / 2, centerY + (height + 1) / 2),
				cv::Range(centerX - width / 2, centerX + (width + 1) / 2));
		cv::imwrite(cropImagePath, cropImage);
	}
}

void ImageResizer::resize(const cv::Mat& src, cv::Mat* pDst, int maxLength) {
	int rows = src.rows;
	int cols = src.cols;
	*pDst = cv::Mat();
	if (rows == 0 || cols == 0) {
		return;
	}
	int length = rows > cols ? rows : cols;
	double ratio = (double) maxLength / length;
	cv::resize(src, *pDst, cv::Size(), ratio, ratio, cv::INTER_LINEAR);
}
