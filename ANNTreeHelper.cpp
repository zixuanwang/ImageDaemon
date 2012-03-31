/*
 * ANNTreeHelper.cpp
 *
 *  Created on: Mar 24, 2012
 *      Author: zixuan
 */

#include "ANNTreeHelper.h"
ANNTreeHelper* ANNTreeHelper::pInstance = NULL;

ANNTreeHelper::ANNTreeHelper() :
		mTreeIndex(0) {

}

ANNTreeHelper* ANNTreeHelper::instance() {
	if (pInstance == NULL) {
		pInstance = new ANNTreeHelper;
	}
	return pInstance;
}

void ANNTreeHelper::buildAllCategory() {
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	std::vector<std::string> columnNameArray;
	columnNameArray.push_back(GlobalConfig::CATEGORY_INDEX_NEXT_COLUMN);
	int scanner = dbAdapter->scannerOpen(GlobalConfig::CATEGORY_INDEX_TABLE, "",
			columnNameArray);
	while (true) {
		std::string strRowKey;
		std::map<std::string, std::string> rowMap;
		dbAdapter->scannerGet(&strRowKey, &rowMap, scanner);
		if (rowMap.empty()) {
			break;
		}
		std::vector<int64_t> sampleImageKeyArray;
		sampleFromCategory(&sampleImageKeyArray, strRowKey, 10000);
		boost::shared_ptr<Feature> pFeature(
				new ColorFeature(GlobalConfig::COLOR_FEATURE_BINSIZE));
		buildCategoryTree(sampleImageKeyArray, strRowKey, pFeature);
		rankCategory(strRowKey, pFeature,
				GlobalConfig::IMAGE_COLOR_FEATURE_COLUMN);
		pFeature.reset(
				new ShapeFeature(GlobalConfig::SHAPE_FEATURE_BINSIZE,
						GlobalConfig::SHAPE_FEATURE_BINSIZE));
		buildCategoryTree(sampleImageKeyArray, strRowKey, pFeature);
		rankCategory(strRowKey, pFeature,
				GlobalConfig::IMAGE_SHAPE_FEATURE_COLUMN);
		pFeature.reset(
				new SURFFeature(GlobalConfig::SURF_FEATURE_COUNT_PER_IMAGE));
		buildCategoryTree(sampleImageKeyArray, strRowKey, pFeature);
		rankCategory(strRowKey, pFeature,
				GlobalConfig::IMAGE_SURF_FEATURE_COLUMN);

	}
	dbAdapter->scannerClose(scanner);
}

bool ANNTreeHelper::buildCategoryTree(
		const std::vector<int64_t>& sampleImageKeyArray,
		const std::string& strRowKey, boost::shared_ptr<Feature> pFeature) {
	std::vector<float> dataArray;
	int sampleCount = 0;
	for (size_t i = 0; i < sampleImageKeyArray.size(); ++i) {
		pFeature->load(sampleImageKeyArray[i]);
		if (!pFeature->empty()) {
			std::vector<float> feature;
			pFeature->getFeature(&feature, 0);
			std::copy(feature.begin(), feature.end(),
					std::back_inserter(dataArray));
			++sampleCount;
		}
	}
	if (sampleCount == 0) {
		return false;
	}
	boost::shared_ptr<ANNTreeRoot> pRoot;
	ANNTreeRootPool::instance()->get(&pRoot, mTreeIndex);
	pRoot->loadSample(dataArray, sampleCount);
	// build ANNTree
	int columnIndex = 0;
	while (true) {
		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
		std::string string;
		dbAdapter->loadCell(
				&string,
				GlobalConfig::CATEGORY_INDEX_TABLE,
				strRowKey,
				GlobalConfig::COLUMN_FAMILY
						+ boost::lexical_cast<std::string>(columnIndex));
		if (string.empty()) {
			break;
		}
		std::vector<int64_t> imageKeyArray;
		TypeConverter::readStringToArray(&imageKeyArray, string);
		for (size_t i = 0; i < imageKeyArray.size(); ++i) {
			int64_t imageKey = imageKeyArray[i];
			pFeature->load(imageKey);
			pFeature->add(pRoot, imageKey);
		}
		++columnIndex;
	}
	pRoot->index();
	return true;
}

void ANNTreeHelper::rankCategory(const std::string& strRowKey,
		boost::shared_ptr<Feature> pFeature, const std::string& columnPrefix) {
	int columnIndex = 0;
	while (true) {
		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
		std::string string;
		dbAdapter->loadCell(
				&string,
				GlobalConfig::CATEGORY_INDEX_TABLE,
				strRowKey,
				GlobalConfig::COLUMN_FAMILY
						+ boost::lexical_cast<std::string>(columnIndex));
		if (string.empty()) {
			break;
		}
		boost::shared_ptr<ANNTreeRoot> pRoot;
		ANNTreeRootPool::instance()->get(&pRoot, mTreeIndex);
		std::vector<int64_t> imageKeyArray;
		TypeConverter::readStringToArray(&imageKeyArray, string);
		for (size_t i = 0; i < imageKeyArray.size(); ++i) {
			std::vector<Neighbor> neighborArray;
			int64_t imageKey = imageKeyArray[i];
			pFeature->load(imageKey);
			pFeature->knnSearch(&neighborArray, pRoot,
					GlobalConfig::STATIC_RANK_SIZE);
			// save rank
			if (neighborArray.empty()) {
				continue;
			}
			int columnCount = (int) neighborArray.size()
					/ GlobalConfig::STATIC_RANK_SIZE_PER_PAGE + 1;
			for (int j = 0; j < columnCount; ++j) {
				std::string strIdArray;
				for (int k = 0; k < GlobalConfig::STATIC_RANK_SIZE_PER_PAGE;
						++k) {
					int index = j * GlobalConfig::STATIC_RANK_SIZE_PER_PAGE + k;
					if (index < (int) neighborArray.size()) {
						std::string strId = boost::lexical_cast<std::string>(
								neighborArray[index].id);
						strIdArray += strId + "|";
					}
				}
				dbAdapter->saveCell(strIdArray.substr(0, strIdArray.size() - 1),
						GlobalConfig::IMAGE_TABLE, imageKey,
						columnPrefix + boost::lexical_cast<std::string>(j));
			}
		}
		++columnIndex;
	}
	ANNTreeRootPool::instance()->put(mTreeIndex);
}

void ANNTreeHelper::sampleFromCategory(
		std::vector<int64_t>* pSampleImageKeyArray,
		const std::string& strRowKey, int sampleSize) {
	// first pass, sampling
	pSampleImageKeyArray->clear();
	int columnIndex = 0;
	srand(time(NULL));
	pSampleImageKeyArray->reserve(sampleSize);
	int imageKeyCount = 0;
	while (true) {
		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
		std::string string;
		dbAdapter->loadCell(
				&string,
				GlobalConfig::CATEGORY_INDEX_TABLE,
				strRowKey,
				GlobalConfig::COLUMN_FAMILY
						+ boost::lexical_cast<std::string>(columnIndex));
		if (string.empty()) {
			break;
		}
		std::vector<int64_t> imageKeyArray;
		TypeConverter::readStringToArray(&imageKeyArray, string);
		for (size_t i = 0; i < imageKeyArray.size(); ++i) {
			if (imageKeyCount < sampleSize) {
				pSampleImageKeyArray->push_back(imageKeyArray[i]);
			} else if ((float) rand() / RAND_MAX
					< (float) sampleSize / imageKeyCount) {
				int dropIndex = rand() % sampleSize;
				(*pSampleImageKeyArray)[dropIndex] = imageKeyArray[i];
			}
			++imageKeyCount;
		}
		++columnIndex;
	}
	std::set<int64_t> uniqueSet(pSampleImageKeyArray->begin(),
			pSampleImageKeyArray->end());
	pSampleImageKeyArray->assign(uniqueSet.begin(), uniqueSet.end());
}

//void ANNTreeHelper::buildOneCategory(const std::string& strRowKey,
//		const std::string& featureColumn) {
//	std::vector<int64_t> sampleImageKeyArray;
//	sampleFromCategory(&sampleImageKeyArray, strRowKey, 10000);
//	// remove duplicate elements
//	std::set<int64_t> uniqueSet(sampleImageKeyArray.begin(),
//			sampleImageKeyArray.end());
//	// load feature data;
//	std::vector<float> dataArray;
//	int sampleCount = 0;
//	for (std::set<int64_t>::iterator iter = uniqueSet.begin();
//			iter != uniqueSet.end(); ++iter) {
//		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
//		std::string strImageKey;
//		TypeConverter::readNumToString(&strImageKey, *iter);
//		std::string string;
//		dbAdapter->loadCell(&string, GlobalConfig::IMAGE_TABLE, strImageKey,
//				featureColumn);
//		if (!string.empty()) {
//			std::vector<float> data;
//			TypeConverter::readStringToArray(&data, string);
//			std::copy(data.begin(), data.end(), std::back_inserter(dataArray));
//			++sampleCount;
//		}
//	}
//	if (sampleCount == 0) {
//		return;
//	}
//	boost::shared_ptr<ANNTreeRoot> pRoot;
//	ANNTreeRootPool::instance()->get(&pRoot, mTreeIndex);
//	pRoot->loadSample(dataArray, sampleCount);
//	// second pass, build slave structure
//	int columnIndex = 0;
//	while (true) {
//		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
//		std::string string;
//		dbAdapter->loadCell(
//				&string,
//				GlobalConfig::CATEGORY_INDEX_TABLE,
//				strRowKey,
//				GlobalConfig::COLUMN_FAMILY
//						+ boost::lexical_cast<std::string>(columnIndex));
//		if (string.empty()) {
//			break;
//		}
//		std::vector<int64_t> imageKeyArray;
//		TypeConverter::readStringToArray(&imageKeyArray, string);
//		for (size_t i = 0; i < imageKeyArray.size(); ++i) {
//			int64_t imageKey = imageKeyArray[i];
//			std::string strImageKey;
//			TypeConverter::readNumToString(&strImageKey, imageKey);
//			std::string strFeature;
//			dbAdapter->loadCell(&strFeature, GlobalConfig::IMAGE_TABLE,
//					strImageKey, featureColumn);
//			std::vector<float> fFeature;
//			TypeConverter::readStringToArray(&fFeature, strFeature);
//			pRoot->addFeature(imageKey, fFeature);
//		}
//		++columnIndex;
//	}
//	pRoot->index();
//}
//
//void ANNTreeHelper::rankOneCategory(const std::string& strRowKey,
//		const std::string& featureColumn) {
//	int columnIndex = 0;
//	while (true) {
//		boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
//		std::string string;
//		dbAdapter->loadCell(
//				&string,
//				GlobalConfig::CATEGORY_INDEX_TABLE,
//				strRowKey,
//				GlobalConfig::COLUMN_FAMILY
//						+ boost::lexical_cast<std::string>(columnIndex));
//		if (string.empty()) {
//			break;
//		}
//		boost::shared_ptr<ANNTreeRoot> pRoot;
//		ANNTreeRootPool::instance()->get(&pRoot, mTreeIndex);
//		std::vector<int64_t> imageKeyArray;
//		TypeConverter::readStringToArray(&imageKeyArray, string);
//		for (size_t i = 0; i < imageKeyArray.size(); ++i) {
//			int64_t imageKey = imageKeyArray[i];
//			std::string strImageKey;
//			TypeConverter::readNumToString(&strImageKey, imageKey);
//			std::string strFeature;
//			dbAdapter->loadCell(&strFeature, GlobalConfig::IMAGE_TABLE,
//					strImageKey, featureColumn);
//			std::vector<float> fFeature;
//			TypeConverter::readStringToArray(&fFeature, strFeature);
//			std::vector<Neighbor> neighborArray;
//			if (featureColumn == GlobalConfig::IMAGE_SURF_FEATURE_COLUMN) {
//
//			} else {
//				pRoot->knnSearch(&neighborArray, fFeature,
//						GlobalConfig::STATIC_RANK_SIZE);
//			}
//			// save rank
//			if (neighborArray.empty()) {
//				continue;
//			}
//			int columnCount = (int) neighborArray.size()
//					/ GlobalConfig::STATIC_RANK_SIZE_PER_PAGE + 1;
//			for (int j = 0; j < columnCount; ++j) {
//				std::string strIdArray;
//				for (int k = 0; k < GlobalConfig::STATIC_RANK_SIZE_PER_PAGE;
//						++k) {
//					int index = j * GlobalConfig::STATIC_RANK_SIZE_PER_PAGE + k;
//					if (index < (int) neighborArray.size()) {
//						std::string strId = boost::lexical_cast<std::string>(
//								neighborArray[index].id);
//						strIdArray += strId + "|";
//					}
//				}
//				dbAdapter->saveCell(
//						strIdArray.substr(0, strIdArray.size() - 1),
//						GlobalConfig::IMAGE_TABLE,
//						strImageKey,
//						featureColumn + "_r"
//								+ boost::lexical_cast<std::string>(j));
//			}
//		}
//		++columnIndex;
//	}
//}

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
				boost::shared_ptr<Feature> pColorFeature(
						new ColorFeature(GlobalConfig::COLOR_FEATURE_BINSIZE));
				boost::shared_ptr<Feature> pShapeFeature(
						new ShapeFeature(GlobalConfig::SHAPE_FEATURE_BINSIZE,
								GlobalConfig::SHAPE_FEATURE_BINSIZE));
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
	int colorTreeIndex = 0;
	int shapeTreeIndex = 1;
	boost::shared_ptr<ANNTreeRoot> pColorRoot;
	boost::shared_ptr<ANNTreeRoot> pShapeRoot;
	ANNTreeRootPool::instance()->get(&pColorRoot, colorTreeIndex);
	ANNTreeRootPool::instance()->get(&pShapeRoot, shapeTreeIndex);
	int colorSampleCount = 0;
	std::vector<float> colorSampleArray;
	for (boost::unordered_map<int, std::vector<float> >::iterator iter =
			colorFeatureMap.begin();
			iter != colorFeatureMap.end() && colorSampleCount < 2;
			++iter, ++colorSampleCount) {
		std::copy(iter->second.begin(), iter->second.end(),
				std::back_inserter(colorSampleArray));
	}
	pColorRoot->loadSample(colorSampleArray, colorSampleCount);
	for (boost::unordered_map<int, std::vector<float> >::iterator iter =
			colorFeatureMap.begin(); iter != colorFeatureMap.end(); ++iter) {
		pColorRoot->addFeature(iter->first, iter->second);
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
	pShapeRoot->loadSample(shapeSampleArray, shapeSampleCount);
	for (boost::unordered_map<int, std::vector<float> >::iterator iter =
			shapeFeatureMap.begin(); iter != shapeFeatureMap.end(); ++iter) {
		pShapeRoot->addFeature(iter->first, iter->second);
	}
	pColorRoot->index();
	pShapeRoot->index();
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
		boost::shared_ptr<ANNTreeRoot> pRoot;
		ANNTreeRootPool::instance()->get(&pRoot, treeIndex);
		pRoot->knnSearch(pReturn, feature, k);
	}
	mysql_free_result(result);
	mysql_close(connect);
}
