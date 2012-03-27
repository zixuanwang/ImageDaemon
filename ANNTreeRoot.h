/*
 * ANNTreeRoot.h
 *
 *  Created on: Feb 19, 2012
 *      Author: zixuan
 */

#ifndef ANNTREEROOT_H_
#define ANNTREEROOT_H_

#include "GlobalConfig.h"
#include "Hbase.h"
#include "ANNTreeDaemon.h"
#include "TypeConverter.h"
#include "Logger.h"
#include "HbaseAdapter.h"
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::hadoop::hbase::thrift;
using namespace ::net::walnutvision;
/// The root structure of ANNTree
class ANNTreeRoot {
public:
	static ANNTreeRoot* instance();
	/// Initialization. This function is NOT thread safe and should be called only once.
	void init(const std::string& confPath, int slavePort);
	/// Clear all data in the root and slaves
	void clear();
	/// Add one new tree. Return the index of the new tree.
	int addTree();
	int treeCount();
	/// Initialize the root with samples. Usually the number of samples is 10000
	void loadSample(int treeIndex, const std::vector<float>& sampleArray,
			int sampleCount);
	/// Add one feature to the ANNTree. This can be only called after loadSample();
	void addFeature(int treeIndex, int64_t id,
			const std::vector<float>& feature);
	/// Create ANN structure on each slave. Called after adding all features.
	void index(int treeIndex);
	void knnSearch(std::vector<Neighbor>* pReturn, int treeIndex,
			const std::vector<float>& feature, int k);
	// Sample from a row in Hbase
//	void sampleFromCategory(std::vector<int64_t>* pSampleImageKeyArray,
//			const std::string& strRowKey);
	// Image ids belong to the category stored in a row in Hbase
//	void buildFromCategory(const std::string& strRowKey, const std::string& featureColumn);
	// Compute the static ranking within each category
//	void rankFromCategory(const std::string& strRowKey, const std::string& featureColumn);

	// obsolete functions
	/// This is a batch process
//	void build();
//	/// This is a real time call
//	void addFeature(int64_t id, const std::string& feature);
	// This is used for face feature
//	void buildFeature(const std::string& type,
//			const std::string& featureDirectory,
//			const std::string& imageDirectory,
//			const std::string& listDirectory);
//	void similarSearch(std::vector<Neighbor>* pReturn, int64_t id, int k,
//			const std::string& type);
private:
	ANNTreeRoot();
	ANNTreeRoot(const ANNTreeRoot&);
	ANNTreeRoot& operator=(const ANNTreeRoot&);
	void loadConfiguration(const std::string& confPath);
	void getSlave(std::vector<int>* pSlaveArray, int treeIndex,
			const std::vector<float>& feature);
	static ANNTreeRoot* pInstance;
	std::vector<std::string> mSlaveNameArray;
	std::vector<boost::shared_ptr<ANNTreeDaemonClient> > mSlaveArray;
	std::vector<cv::Mat> mMatArray;
	std::vector<boost::shared_ptr<cv::flann::Index> > mpIndexArray;
	std::vector<std::vector<int> > mLabelArray;
	static pthread_mutex_t sMutex;
	std::string mConfPath;
	struct NeighborComparator {
		bool operator()(const Neighbor &i, const Neighbor &j) {
			return (i.distance < j.distance);
		}
	};
};

#endif /* ANNTREEROOT_H_ */
