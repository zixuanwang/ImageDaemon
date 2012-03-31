/*
 * ANNTreeHelper.h
 *
 *  Created on: Mar 24, 2012
 *      Author: zixuan
 */

#ifndef ANNTREEHELPER_H_
#define ANNTREEHELPER_H_
#include "GlobalConfig.h"
#include "ANNTreeSlavePool.h"
#include "ANNTreeRootPool.h"
#include "ShapeFeature.h"
#include "ColorFeature.h"
#include "SURFFeature.h"

/// This class provides some helper functions to build the ANNTree.
class ANNTreeHelper {
public:
	static ANNTreeHelper* instance();
	void sampleFromCategory(std::vector<int64_t>* pSampleImageKeyArray,
			const std::string& strRowKey, int sampleSize);
	void buildAllCategory();
	bool buildCategoryTree(const std::vector<int64_t>& sampleImageKeyArray,
			const std::string& strRowKey, boost::shared_ptr<Feature> pFeature);
	void rankCategory(const std::string& strRowKey,
			boost::shared_ptr<Feature> pFeature, const std::string& columnPrefix);
//	void buildOneCategory(const std::string& strRowKey,
//			const std::string& featureColumn);
//	void rankOneCategory(const std::string& strRowKey,
//			const std::string& featureColumn);
	void similarSearch(std::vector<Neighbor>* pReturn, int treeIndex,
			int64_t id, int k);
	void computeYPTree();
private:
	ANNTreeHelper();
	ANNTreeHelper(const ANNTreeHelper&);
	ANNTreeHelper& operator=(const ANNTreeHelper&);
	static ANNTreeHelper* pInstance;

	// temporary variable
	int mTreeIndex;
};

#endif /* ANNTREEHELPER_H_ */
