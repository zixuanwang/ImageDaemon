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
#include "ApplyBuildCategoryTree.h"

/// This class provides some helper functions to build the ANNTree.
class ANNTreeHelper {
public:
	static ANNTreeHelper* instance();
	void buildAllCategory() const;
	void buildCategory(const std::string& categoryName) const;
	void similarSearch(std::vector<Neighbor>* pReturn, int treeIndex,
			int64_t id, int k);
	void computeYPTree();
private:
	ANNTreeHelper();
	ANNTreeHelper(const ANNTreeHelper&);
	ANNTreeHelper& operator=(const ANNTreeHelper&);
	static ANNTreeHelper* pInstance;
};

#endif /* ANNTREEHELPER_H_ */
