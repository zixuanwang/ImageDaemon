/*
 * ANNTreeSlavePool.cpp
 *
 *  Created on: Mar 27, 2012
 *      Author: zixuan
 */

#include "ANNTreeSlavePool.h"
ANNTreeSlavePool* ANNTreeSlavePool::pInstance = NULL;

ANNTreeSlavePool::ANNTreeSlavePool() {

}

ANNTreeSlavePool* ANNTreeSlavePool::instance() {
	if (pInstance == NULL) {
		pInstance = new ANNTreeSlavePool;
	}
	return pInstance;
}

void ANNTreeSlavePool::get(boost::shared_ptr<ANNTreeSlave>* pSlave,
		int treeIndex) {
	boost::mutex::scoped_lock lock(mMutex);
	if (mPool.find(treeIndex) == mPool.end()) {
		mPool[treeIndex] = boost::shared_ptr<ANNTreeSlave>(new ANNTreeSlave(treeIndex));
	}
	*pSlave = mPool[treeIndex];
}

void ANNTreeSlavePool::put(int treeIndex) {
	boost::mutex::scoped_lock lock(mMutex);
	mPool.erase(treeIndex);
}

void ANNTreeSlavePool::clear() {
	boost::mutex::scoped_lock lock(mMutex);
	mPool.clear();
}
