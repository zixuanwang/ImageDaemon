/*
 * ANNTreeSlavePool.h
 *
 *  Created on: Mar 27, 2012
 *      Author: zixuan
 */

#ifndef ANNTREESLAVEPOOL_H_
#define ANNTREESLAVEPOOL_H_
#include "ANNTreeSlave.h"
class ANNTreeSlavePool {
public:
	static ANNTreeSlavePool* instance();
	/// Get a pointer to a tree slave.
	void get(boost::shared_ptr<ANNTreeSlave>* pSlave, int treeIndex);
	/// Release the resource.
	void put(int treeIndex);
	/// Clear all trees in the pool
	void clear();
private:
	static ANNTreeSlavePool* pInstance;
	ANNTreeSlavePool();
	ANNTreeSlavePool(const ANNTreeSlavePool&);
	ANNTreeSlavePool& operator=(const ANNTreeSlavePool&);
	boost::unordered_map<int, boost::shared_ptr<ANNTreeSlave> > mPool;
	boost::mutex mMutex;
};

#endif /* ANNTREESLAVEPOOL_H_ */
