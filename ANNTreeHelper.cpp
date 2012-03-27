/*
 * ANNTreeHelper.cpp
 *
 *  Created on: Mar 24, 2012
 *      Author: zixuan
 */

#include "ANNTreeHelper.h"
ANNTreeHelper* ANNTreeHelper::pInstance = NULL;

ANNTreeHelper::ANNTreeHelper() {

}

ANNTreeHelper* ANNTreeHelper::instance() {
	if (pInstance == NULL) {
		pInstance = new ANNTreeHelper;
	}
	return pInstance;
}

void ANNTreeHelper::computeYPTree() {
	std::string dbFilePath = "/home/zixuan/public_html/bags/dbfile";
	std::string imageDirectory = "/home/zixuan/public_html/bags/imgs/";
	std::string type = "bags";
	boost::unordered_map<int, std::vector<float> > colorFeatureMap;
	boost::unordered_map<int, std::vector<float> > shapeFeatureMap;
	// compute feature
	// save to mysql
	MYSQL * connect; // Create a pointer to the MySQL instance
	connect = mysql_init(NULL);
	connect = mysql_real_connect(connect, "localhost", "root", "myhome515",
			"image", 0, NULL, 0);
	std::string sqlString =
			"CREATE TABLE " + type
					+ " (id INT, url varchar(255) NOT NULL, price FLOAT, name varchar(255), feature_color MEDIUMBLOB, feature_shape MEDIUMBLOB, PRIMARY KEY(id))";
	mysql_query(connect, sqlString.c_str());
	std::ifstream dbFileInStream;
	dbFileInStream.open(dbFilePath.c_str());
	if (dbFileInStream.good()) {
		std::string line;
		while (getline(dbFileInStream, line)) {
			std::vector<std::string> tokenArray;
			boost::split(tokenArray, line, boost::is_any_of(","));
			if (tokenArray.size() == 4) {
				std::string id = tokenArray[0];
				std::string url = tokenArray[1];
				std::string price = tokenArray[2];
				std::string name = tokenArray[3];
				std::string imagePath = imageDirectory + "image" + id + ".jpg";
				cv::Mat image = cv::imread(imagePath, 1);
				boost::shared_ptr<Feature> pColorFeature(new ColorFeature(8));
				boost::shared_ptr<Feature> pShapeFeature(
						new ShapeFeature(10, 10));
				pColorFeature->compute(image);
				pShapeFeature->compute(image);
				std::string strColorFeature;
				pColorFeature->save(&strColorFeature);
				std::string strShapeFeature;
				pShapeFeature->save(&strShapeFeature);
				int iId = atoi(id.c_str());
				float fPrice = atof(price.c_str());
				colorFeatureMap[iId] = pColorFeature->getFeature();
				shapeFeatureMap[iId] = pShapeFeature->getFeature();
				char colorChunk[10 * 1024];
				char query[20 * 1024];
				char shapeChunk[10 * 1024];
				mysql_real_escape_string(connect, colorChunk,
						(char*) &strColorFeature[0], strColorFeature.size());
				mysql_real_escape_string(connect, shapeChunk,
						(char*) &strShapeFeature[0], strShapeFeature.size());
				std::string sqlString =
						"INSERT INTO " + type
								+ " (id, url, price, name, feature_color, feature_shape) VALUES('%d', '%s', '%f', '%s', '%s', '%s')";
				int len = snprintf(query, sizeof(query), sqlString.c_str(), iId,
						url.c_str(), fPrice, name.c_str(), colorChunk,
						shapeChunk);
				mysql_real_query(connect, query, len);
			}
			std::cout << "Computing feature for " << line << std::endl;
		}
		dbFileInStream.close();
	}
	mysql_close(connect);
	// build the tree
	ANNTreeRoot::instance()->clear();
	int colorTreeIndex = ANNTreeRoot::instance()->addTree();
	int shapeTreeIndex = ANNTreeRoot::instance()->addTree();
	int colorSampleCount = 0;
	std::vector<float> colorSampleArray;
	for (boost::unordered_map<int, std::vector<float> >::iterator iter =
			colorFeatureMap.begin();
			iter != colorFeatureMap.end() && colorSampleCount < 2;
			++iter, ++colorSampleCount) {
		std::copy(iter->second.begin(), iter->second.end(),
				std::back_inserter(colorSampleArray));
	}
	ANNTreeRoot::instance()->loadSample(colorTreeIndex, colorSampleArray,
			colorSampleCount);
	for (boost::unordered_map<int, std::vector<float> >::iterator iter =
			colorFeatureMap.begin(); iter != colorFeatureMap.end(); ++iter) {
		ANNTreeRoot::instance()->addFeature(colorTreeIndex, iter->first,
				iter->second);
	}
	int shapeSampleCount = 0;
	std::vector<float> shapeSampleArray;
	for (boost::unordered_map<int, std::vector<float> >::iterator iter =
			shapeFeatureMap.begin();
			iter != shapeFeatureMap.end() && shapeSampleCount < 2;
			++iter, ++shapeSampleCount) {
		std::copy(iter->second.begin(), iter->second.end(),
				std::back_inserter(shapeSampleArray));
	}
	ANNTreeRoot::instance()->loadSample(shapeTreeIndex, shapeSampleArray,
			shapeSampleCount);
	for (boost::unordered_map<int, std::vector<float> >::iterator iter =
			shapeFeatureMap.begin(); iter != shapeFeatureMap.end(); ++iter) {
		ANNTreeRoot::instance()->addFeature(shapeTreeIndex, iter->first,
				iter->second);
	}
	ANNTreeRoot::instance()->index(colorTreeIndex);
	ANNTreeRoot::instance()->index(shapeTreeIndex);
}

void ANNTreeHelper::similarSearch(std::vector<Neighbor>* pReturn, int treeIndex,
		int64_t id, int k) {
	pReturn->clear();
	std::string type = "bags";
	MYSQL *connect;
	connect = mysql_init(NULL);
	connect = mysql_real_connect(connect, "localhost", "root", "myhome515",
			"image", 0, NULL, 0);
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned long *lengths;
	std::string queryString;
	if (treeIndex == 0) {
		queryString = "SELECT feature_color FROM " + type + " WHERE id = "
				+ boost::lexical_cast<std::string>(id);
	} else {
		queryString = "SELECT feature_shape FROM " + type + " WHERE id = "
				+ boost::lexical_cast<std::string>(id);
	}
	if (mysql_query(connect, queryString.c_str()) != 0) {
		return;
	}
	result = mysql_store_result(connect);
	if (result == NULL) {
		return;
	}
	row = mysql_fetch_row(result);
	if (row == NULL) {
		return;
	}
	lengths = mysql_fetch_lengths(result);
	if (lengths[0] > 0) {
		std::vector<float> feature(lengths[0] / sizeof(float));
		memcpy((char*) &feature[0], row[0], lengths[0]);
		ANNTreeRoot::instance()->knnSearch(pReturn, treeIndex, feature, k);
	}
	mysql_free_result(result);
	mysql_close(connect);
}
