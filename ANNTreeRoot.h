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
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::hadoop::hbase::thrift;
using namespace ANNTreeDaemon;
class ANNTreeRoot {
public:
	static ANNTreeRoot* instance();
	void init(const std::string& confPath, int slavePort);
	/// This is a batch process
	void build();
	/// This is a real time call
	void addFeature(int64_t id, const std::string& feature);
	///
	void knnSearch(std::vector<Neighbor>* pReturn, const std::string& feature,
			int k);

	// This is used for face feature
	void buildFeature(const std::string& type,
			const std::string& featureDirectory,
			const std::string& imageDirectory,
			const std::string& listDirectory);
	void similarSearch(std::vector<Neighbor>* pReturn, int64_t id, int k, const std::string& type);
private:
	ANNTreeRoot();
	ANNTreeRoot(const ANNTreeRoot&);
	ANNTreeRoot& operator=(const ANNTreeRoot&);
	void loadConfiguration(const std::string& confPath);
	/// This function returns the actual sample count.
	int selectSeed(int sampleCount);
	void getSlave(std::vector<int>* pSlaveArray, const std::string& feature);
	static ANNTreeRoot* pInstance;
	std::vector<std::string> mSlaveNameArray;
	std::vector<boost::shared_ptr<ANNTreeDaemonClient> > mClientArray;
	cv::Mat mMat;
	boost::shared_ptr<cv::flann::Index> mpIndex;
	static pthread_mutex_t sMutex;
	std::string mConfPath;
};

#endif /* ANNTREEROOT_H_ */
