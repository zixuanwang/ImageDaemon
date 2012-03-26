/*
 * ANNTreeHelper.h
 *
 *  Created on: Mar 24, 2012
 *      Author: zixuan
 */

#ifndef ANNTREEHELPER_H_
#define ANNTREEHELPER_H_
#include "GlobalConfig.h"
#include "ANNTreeRoot.h"
#include "ShapeFeature.h"
#include "ColorFeature.h"

/// This class provides some helper functions to build the ANNTree.
class ANNTreeHelper {
public:
	static ANNTreeHelper* instance();
	void computeYPTree();
	void similarSearch(std::vector<Neighbor>* pReturn, int treeIndex,
			int64_t id, int k);
private:
	ANNTreeHelper();
	ANNTreeHelper(const ANNTreeHelper&);
	ANNTreeHelper& operator=(const ANNTreeHelper&);
	static ANNTreeHelper* pInstance;
};

#endif /* ANNTREEHELPER_H_ */
