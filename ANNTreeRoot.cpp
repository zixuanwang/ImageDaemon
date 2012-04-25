/*
 * ANNTreeRoot.cpp
 *
 *  Created on: Feb 19, 2012
 *      Author: zixuan
 */

#include "ANNTreeRoot.h"

std::vector<std::string> ANNTreeRoot::sSlaveNameArray;
int ANNTreeRoot::sSlavePort = 0;
std::string ANNTreeRoot::sConfPath;
boost::mutex ANNTreeRoot::sMutex;

ANNTreeRoot::ANNTreeRoot(int treeIndex) :
		mTreeIndex(treeIndex) {
}

ANNTreeRoot::~ANNTreeRoot() {
	// delete corresponding slaves
	for (size_t i = 0; i < sSlaveNameArray.size(); ++i) {
		ScopedANNTreeDaemonClient client(sSlaveNameArray[i], sSlavePort);
		try {
			client.client->slavePutTree(mTreeIndex);
		} catch (const TException& tx) {
			std::cerr << tx.what() << std::endl;
		}
	}
}

void ANNTreeRoot::init(const std::string& confPath, int slavePort) {
	boost::mutex::scoped_lock lock(sMutex);
	// load configuration
	std::ifstream inStream;
	inStream.open(confPath.c_str());
	if (inStream.good()) {
		std::string server;
		while (getline(inStream, server)) {
			sSlaveNameArray.push_back(server);
		}
		inStream.close();
		sConfPath = confPath;
	}
	if (sSlaveNameArray.empty()) {
		std::cerr << "Error in loading configuration file in ANNTreeRoot"
				<< std::endl;
		return;
	}
	sSlavePort = slavePort;
	// start slave processes
//	std::string cmd = "parallel-ssh -h " + sConfPath
//			+ " -t 0 \"ANNTreeDaemon -c " + sConfPath + " -p "
//			+ boost::lexical_cast<std::string>(slavePort)
//			+ " > /tmp/anntree_slave.log\" &";
//	if (system(cmd.c_str()) != 0) {
//		std::cerr << "Error in starting ANNTreeSlave." << std::endl;
//		return;
//	}
//	sleep(3);
}

void ANNTreeRoot::loadSample(const std::vector<float>& sampleArray,
		int sampleCount) {
	boost::mutex::scoped_lock lock(mMutex);
	if (sampleArray.empty() || sampleCount == 0) {
		std::cerr << "Wrong sampleCount or sampleArray size." << std::endl;
		return;
	}
	mMat = cv::Mat(sampleArray, true);
	mMat = mMat.reshape(0, sampleCount);
	mpIndex.reset(new cv::flann::Index(mMat, cv::flann::KDTreeIndexParams(8)));
	// clustering samples
	if (sampleCount > (int) sSlaveNameArray.size()) {
		// OpenCV kmeans has bug when clustering one point for one cluster
		cv::Mat clusters = cv::Mat(mMat.rows, 1, CV_32SC1);
		cv::kmeans(mMat, sSlaveNameArray.size(), clusters,
				cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 100, 1e-6),
				5, cv::KMEANS_PP_CENTERS);
		int* ptr = clusters.ptr<int>(0);
		mLabelArray.assign(ptr, ptr + clusters.rows);
	} else {
		mLabelArray.assign(sampleCount, 0);
		for (int i = 1; i < sampleCount; ++i) {
			mLabelArray[i] = i;
		}
	}
}

void ANNTreeRoot::getSlave(std::vector<int>* pSlaveArray,
		const std::vector<float>& feature) {
	pSlaveArray->clear();
	if (mMat.empty() || feature.empty()) {
		return;
	}
	int k = mMat.rows;
	k = k < 2 ? k : 2;
	std::vector<int> indices(k);
	std::vector<float> dists(k);
	mpIndex->knnSearch(feature, indices, dists, k, cv::flann::SearchParams(32));
	pSlaveArray->push_back(mLabelArray[indices[0]]);
	// dealing with points near Voronoi edges
	if (k == 2) {
		if (dists[0] > 0.85 * dists[1]
				&& mLabelArray[indices[0]] != mLabelArray[indices[1]]) {
			pSlaveArray->push_back(mLabelArray[indices[1]]);
		}
	}
}

void ANNTreeRoot::removeDuplicate(std::vector<Neighbor>* pNeighborArray) {
	pNeighborArray->erase(
			std::unique(pNeighborArray->begin(), pNeighborArray->end(),
					NeighborIdComparator()), pNeighborArray->end());

}

void ANNTreeRoot::addFeature(int64_t id, const std::vector<float>& feature) {
	if (mMat.empty()) {
		std::cerr << "Call loadSample() first." << std::endl;
		return;
	}
	std::string strFeature;
	TypeConverter::readArrayToString(&strFeature, feature);
	std::vector<int> slaveArray;
	getSlave(&slaveArray, feature);
	// Each feature is stored in memory only once.
	if (!slaveArray.empty()) {
		ScopedANNTreeDaemonClient client(sSlaveNameArray[slaveArray[0]],
				sSlavePort);
		try {
			client.client->slaveAddFeature(mTreeIndex, id, strFeature);
		} catch (const TException& tx) {
			std::cerr << tx.what() << std::endl;
		}
	}
}

void ANNTreeRoot::index() {
	for (size_t i = 0; i < sSlaveNameArray.size(); ++i) {
		ScopedANNTreeDaemonClient client(sSlaveNameArray[i], sSlavePort);
		try {
			client.client->slaveIndex(mTreeIndex);
		} catch (const TException& tx) {
			std::cerr << tx.what() << std::endl;
		}
	}
}

void ANNTreeRoot::knnSearch(std::vector<Neighbor>* pReturn,
		const std::vector<float>& feature, int k) {
	pReturn->clear();
	if (mMat.empty() || feature.empty() || k < 1) {
		return;
	}
	std::string strFeature;
	TypeConverter::readArrayToString(&strFeature, feature);
	std::vector<int> slaveArray;
	getSlave(&slaveArray, feature);
	if (slaveArray.size() == 1) {
		ScopedANNTreeDaemonClient client(sSlaveNameArray[slaveArray[0]],
				sSlavePort);
		try {
			client.client->slaveKnnSearch(*pReturn, mTreeIndex, strFeature, k);
		} catch (const TException& tx) {
			std::cerr << tx.what() << std::endl;
		}
	} else {
		std::vector<Neighbor> candidateArray1;
		std::vector<Neighbor> candidateArray2;
		if (slaveArray.size() == 2) {
			ScopedANNTreeDaemonClient client0(sSlaveNameArray[slaveArray[0]],
					sSlavePort);
			ScopedANNTreeDaemonClient client1(sSlaveNameArray[slaveArray[1]],
					sSlavePort);
			try {
				client0.client->slaveKnnSearch(candidateArray1, mTreeIndex,
						strFeature, k);
				client1.client->slaveKnnSearch(candidateArray2, mTreeIndex,
						strFeature, k);
			} catch (const TException& tx) {
				std::cerr << tx.what() << std::endl;
			}
		}
		int mergeCandidateArraySize = (int) candidateArray1.size()
				+ (int) candidateArray2.size();
		std::vector<Neighbor> mergeCandidateArray(mergeCandidateArraySize);
		std::merge(candidateArray1.begin(), candidateArray1.end(),
				candidateArray2.begin(), candidateArray2.end(),
				mergeCandidateArray.begin(), NeighborComparator());
		int length = k < mergeCandidateArraySize ? k : mergeCandidateArraySize;
		pReturn->assign(mergeCandidateArray.begin(),
				mergeCandidateArray.begin() + length);
	}
	removeDuplicate(pReturn);
}

void ANNTreeRoot::knnSearch(std::vector<Neighbor>* pReturn,
		const std::vector<std::vector<float> >& featureArray, int k) {
	pReturn->clear();
	if (featureArray.empty() || k < 1) {
		return;
	}
	boost::unordered_map<int64_t, double> idDistanceMap;
	int subK = 5 * k / featureArray.size();
	if (subK < 1) {
		subK = 1;
	}
	for (size_t i = 0; i < featureArray.size(); ++i) {
		std::vector<Neighbor> returnCandidate;
		knnSearch(&returnCandidate, featureArray[i], subK);
		for (size_t j = 0; j < returnCandidate.size(); ++j) {
			idDistanceMap[returnCandidate[j].id] -= exp(
					-1.0 * returnCandidate[j].distance);
		}
	}
	std::vector<RankItem<int64_t, double> > rankList;
	rankList.reserve(idDistanceMap.size());
	for (boost::unordered_map<int64_t, double>::iterator iter =
			idDistanceMap.begin(); iter != idDistanceMap.end(); ++iter) {
		RankItem<int64_t, double> item(iter->first, iter->second);
		rankList.push_back(item);
	}
	int rankListSize = (int) rankList.size();
	if (rankListSize <= k) {
		std::sort(rankList.begin(), rankList.end());
	} else {
		std::partial_sort(rankList.begin(), rankList.begin() + k,
				rankList.end());
	}
	int returnSize = rankListSize < k ? rankListSize : k;
	pReturn->reserve(returnSize);
	for (int i = 0; i < returnSize; ++i) {
		Neighbor neighbor;
		neighbor.id = rankList[i].index;
		neighbor.distance = rankList[i].value;
		pReturn->push_back(neighbor);
	}
	removeDuplicate(pReturn);
}

void ANNTreeRoot::save() {
	boost::mutex::scoped_lock lock(mMutex);
	std::string fileName = GlobalConfig::ANNTREE_ROOT_FILE + "_"
			+ boost::lexical_cast<std::string>(mTreeIndex);
	mpIndex->save(fileName + ".index");
	Serializer::save(mMat, fileName + ".mat");
	Serializer::save(mLabelArray, fileName + ".label");
	for (size_t i = 0; i < sSlaveNameArray.size(); ++i) {
		ScopedANNTreeDaemonClient client(sSlaveNameArray[i], sSlavePort);
		try {
			client.client->slaveSave(mTreeIndex);
		} catch (const TException& tx) {
			std::cerr << tx.what() << std::endl;
		}
	}
}

void ANNTreeRoot::load() {
	boost::mutex::scoped_lock lock(mMutex);
	std::string fileName = GlobalConfig::ANNTREE_ROOT_FILE + "_"
			+ boost::lexical_cast<std::string>(mTreeIndex);
	Serializer::load(&mMat, fileName + ".mat");
	Serializer::load(&mLabelArray, fileName + ".label");
	mpIndex.reset(
			new cv::flann::Index(mMat,
					cv::flann::SavedIndexParams(fileName + ".index")));
	for (size_t i = 0; i < sSlaveNameArray.size(); ++i) {
		ScopedANNTreeDaemonClient client(sSlaveNameArray[i], sSlavePort);
		try {
			client.client->slaveLoad(mTreeIndex);
		} catch (const TException& tx) {
			std::cerr << tx.what() << std::endl;
		}
	}
}
