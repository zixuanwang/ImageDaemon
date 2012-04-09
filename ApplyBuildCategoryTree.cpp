/*
 * ApplyBuildCategoryTree.cpp
 *
 *  Created on: Apr 8, 2012
 *      Author: zixuan
 */

#include "ApplyBuildCategoryTree.h"

ApplyBuildCategoryTree::ApplyBuildCategoryTree(){

}

ApplyBuildCategoryTree::ApplyBuildCategoryTree(
		const std::vector<std::string> categoryNameArray) :
		mCategoryNameArray(categoryNameArray) {

}

ApplyBuildCategoryTree::~ApplyBuildCategoryTree() {

}

void ApplyBuildCategoryTree::buildCategory(const std::string& categoryName) const {
	std::cout << categoryName << std::endl;
	std::vector<int64_t> sampleImageKeyArray;
	sampleFromCategory(&sampleImageKeyArray, categoryName, 10000);
	int colorTreeIndex = getTreeIndex(categoryName,
			GlobalConfig::COLOR_TREEINDEX_COLUMN);
	int shapeTreeIndex = getTreeIndex(categoryName,
			GlobalConfig::SHAPE_TREEINDEX_COLUMN);
	int surfTreeIndex = getTreeIndex(categoryName,
			GlobalConfig::SURF_TREEINDEX_COLUMN);
	boost::shared_ptr<Feature> pFeature(
			new ColorFeature(GlobalConfig::COLOR_FEATURE_BINSIZE));
	buildCategoryTree(sampleImageKeyArray, categoryName, pFeature,
			colorTreeIndex);
//	rankCategory(categoryName, pFeature,
//			GlobalConfig::IMAGE_COLOR_FEATURE_COLUMN, colorTreeIndex);
	pFeature.reset(
			new ShapeFeature(GlobalConfig::SHAPE_FEATURE_BINSIZE,
					GlobalConfig::SHAPE_FEATURE_BINSIZE));
	buildCategoryTree(sampleImageKeyArray, categoryName, pFeature,
			shapeTreeIndex);
//	rankCategory(categoryName, pFeature,
//			GlobalConfig::IMAGE_SHAPE_FEATURE_COLUMN, shapeTreeIndex);
	pFeature.reset(new SURFFeature(GlobalConfig::SURF_FEATURE_COUNT_PER_IMAGE));
	buildCategoryTree(sampleImageKeyArray, categoryName, pFeature,
			surfTreeIndex);
//	rankCategory(categoryName, pFeature,
//			GlobalConfig::IMAGE_SURF_FEATURE_COLUMN, surfTreeIndex);
}

bool ApplyBuildCategoryTree::buildCategoryTree(
		const std::vector<int64_t>& sampleImageKeyArray,
		const std::string& strRowKey, boost::shared_ptr<Feature> pFeature,
		int treeIndex) const {
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
	boost::shared_ptr<ANNTreeRoot> pBuildRoot;
	ANNTreeRootPool::instance()->get(&pBuildRoot, treeIndex);
	pBuildRoot->loadSample(dataArray, sampleCount);
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
		std::set<int64_t> uniqueSet(imageKeyArray.begin(), imageKeyArray.end());
		imageKeyArray.assign(uniqueSet.begin(), uniqueSet.end());
		for (size_t i = 0; i < imageKeyArray.size(); ++i) {
			int64_t imageKey = imageKeyArray[i];
			pFeature->load(imageKey);
			pFeature->add(pBuildRoot, imageKey);
		}
		++columnIndex;
	}
	pBuildRoot->index();
	return true;
}

void ApplyBuildCategoryTree::rankCategory(const std::string& strRowKey,
		boost::shared_ptr<Feature> pFeature, const std::string& columnPrefix,
		int treeIndex) const {
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
		ANNTreeRootPool::instance()->get(&pRoot, treeIndex);
		std::vector<int64_t> imageKeyArray;
		TypeConverter::readStringToArray(&imageKeyArray, string);
		std::set<int64_t> uniqueSet(imageKeyArray.begin(), imageKeyArray.end());
		imageKeyArray.assign(uniqueSet.begin(), uniqueSet.end());
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
}

void ApplyBuildCategoryTree::sampleFromCategory(
		std::vector<int64_t>* pSampleImageKeyArray,
		const std::string& strRowKey, int sampleSize) const {
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

int ApplyBuildCategoryTree::getTreeIndex(const std::string& strRowKey,
		const std::string& columnName) const {
	boost::shared_ptr<DBAdapter> dbAdapter(new HbaseAdapter);
	std::string strId;
	dbAdapter->loadCell(&strId, GlobalConfig::CATEGORY_INDEX_TABLE, strRowKey,
			columnName);
	int id = 0;
	TypeConverter::readStringToNum(&id, strId);
	return id;
}
