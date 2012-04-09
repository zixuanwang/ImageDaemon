/*
 * ApplyBuildCategoryTree.h
 *
 *  Created on: Apr 8, 2012
 *      Author: zixuan
 */

#ifndef APPLYBUILDCATEGORYTREE_H_
#define APPLYBUILDCATEGORYTREE_H_
#include "GlobalConfig.h"
#include "ANNTreeSlavePool.h"
#include "ANNTreeRootPool.h"
#include "ShapeFeature.h"
#include "ColorFeature.h"
#include "SURFFeature.h"
class ApplyBuildCategoryTree {
public:
	ApplyBuildCategoryTree();
	ApplyBuildCategoryTree(const std::vector<std::string> categoryNameArray);
	~ApplyBuildCategoryTree();
	void operator()(const tbb::blocked_range<size_t>& r) const {
		for (size_t i = r.begin(); i != r.end(); ++i) {
			buildCategory(mCategoryNameArray[i]);
		}
	}
	void sampleFromCategory(std::vector<int64_t>* pSampleImageKeyArray,
			const std::string& strRowKey, int sampleSize) const;
	void buildCategory(const std::string& categoryName) const;
	bool buildCategoryTree(const std::vector<int64_t>& sampleImageKeyArray,
			const std::string& strRowKey, boost::shared_ptr<Feature> pFeature,
			int treeIndex) const;
	void rankCategory(const std::string& strRowKey,
			boost::shared_ptr<Feature> pFeature,
			const std::string& columnPrefix, int treeIndex) const;
	int getTreeIndex(const std::string& strRowKey,
			const std::string& columnName) const;
private:
	std::vector<std::string> mCategoryNameArray;
};

#endif /* APPLYBUILDCATEGORYTREE_H_ */
