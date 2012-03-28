/*
 * ANNTreeRootPool.h
 *
 *  Created on: Mar 27, 2012
 *      Author: zixuan
 */

#ifndef ANNTREEROOTPOOL_H_
#define ANNTREEROOTPOOL_H_
#include "ANNTreeRoot.h"

class ANNTreeRootPool {
public:
	static ANNTreeRootPool* instance();
	/// Get a pointer to a tree root.
	void get(boost::shared_ptr<ANNTreeRoot>* pRoot, int treeIndex);
	/// Release the resource.
	void put(int treeIndex);
	/// Clear all trees in the pool
	void clear();
private:
	static ANNTreeRootPool* pInstance;
	ANNTreeRootPool();
	ANNTreeRootPool(const ANNTreeRootPool&);
	ANNTreeRootPool& operator=(const ANNTreeRootPool&);
	boost::unordered_map<int, boost::shared_ptr<ANNTreeRoot> > mPool;
	boost::mutex mMutex;
};

#endif /* ANNTREEROOTPOOL_H_ */
