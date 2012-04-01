/*
 * ANNTreeRoot.h
 *
 *  Created on: Feb 19, 2012
 *      Author: zixuan
 */

#ifndef ANNTREEROOT_H_
#define ANNTREEROOT_H_

#include "GlobalConfig.h"
#include "ANNTreeDaemon.h"
#include "TypeConverter.h"
#include "RankItem.h"
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace ::net::walnutvision;
/// The root structure of ANNTree
class ANNTreeRoot {
public:
	ANNTreeRoot(int treeIndex);
	~ANNTreeRoot();
	/// Initialization. This function is NOT thread safe and should be called only once.
	static void init(const std::string& confPath, int slavePort);
	/// Initialize the root with samples. Usually the number of samples is 10000
	void loadSample(const std::vector<float>& sampleArray, int sampleCount);
	/// Add one feature to the ANNTree. This can be only called after loadSample();
	void addFeature(int64_t id, const std::vector<float>& feature);
	/// Create ANN structure on each slave. Called after adding all features. After index, should not add more features.
	void index();
	/// This is for the regular knn search. e.g. color and shape
	void knnSearch(std::vector<Neighbor>* pReturn,
			const std::vector<float>& feature, int k);
	void knnSearch(std::vector<Neighbor>* pReturn,
			const std::vector<std::vector<float> >& featureArray, int k);
private:
	void getSlave(std::vector<int>* pSlaveArray,
			const std::vector<float>& feature);
	void removeDuplicate(std::vector<Neighbor>* pNeighborArray);
	int mTreeIndex;
	cv::Mat mMat;
	boost::shared_ptr<cv::flann::Index> mpIndex;
	std::vector<int> mLabelArray;
	boost::mutex mMutex;
	// static variables, shared by all instances
	static std::vector<std::string> sSlaveNameArray;
	static std::vector<boost::shared_ptr<ANNTreeDaemonClient> > sSlaveArray;
	static std::string sConfPath;
	struct NeighborComparator {
		bool operator()(const Neighbor &i, const Neighbor &j) {
			return (i.distance < j.distance);
		}
	};
	struct NeighborIdComparator {
		bool operator()(const Neighbor &i, const Neighbor &j) {
			return (i.id == j.id);
		}
	};
};

#endif /* ANNTREEROOT_H_ */
