/*
 * ANNTreeRootPool.cpp
 *
 *  Created on: Mar 27, 2012
 *      Author: zixuan
 */

#include "ANNTreeRootPool.h"

ANNTreeRootPool* ANNTreeRootPool::pInstance = NULL;

ANNTreeRootPool::ANNTreeRootPool() {

}

ANNTreeRootPool* ANNTreeRootPool::instance() {
	if (pInstance == NULL) {
		pInstance = new ANNTreeRootPool;
	}
	return pInstance;
}

void ANNTreeRootPool::get(boost::shared_ptr<ANNTreeRoot>* pRoot,
		int treeIndex) {
	boost::mutex::scoped_lock lock(mMutex);
	if (mPool.find(treeIndex) == mPool.end()) {
		mPool[treeIndex] = boost::shared_ptr<ANNTreeRoot>(
				new ANNTreeRoot(treeIndex));
	}
	*pRoot = mPool[treeIndex];
}

void ANNTreeRootPool::put(int treeIndex) {
	boost::mutex::scoped_lock lock(mMutex);
	mPool.erase(treeIndex);
}

void ANNTreeRootPool::clear() {
	boost::mutex::scoped_lock lock(mMutex);
	mPool.clear();
}

