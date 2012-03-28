/*
 * ANNTreeRoot.cpp
 *
 *  Created on: Feb 19, 2012
 *      Author: zixuan
 */

#include "ANNTreeRoot.h"

std::vector<std::string> ANNTreeRoot::sSlaveNameArray;
std::vector<boost::shared_ptr<ANNTreeDaemonClient> > ANNTreeRoot::sSlaveArray;
std::string ANNTreeRoot::sConfPath;

ANNTreeRoot::ANNTreeRoot(int treeIndex) :
		mTreeIndex(treeIndex) {

}

ANNTreeRoot::~ANNTreeRoot() {
	// delete corresponding slaves
	for (size_t i = 0; i < sSlaveArray.size(); ++i) {
		sSlaveArray[i]->slavePutTree(mTreeIndex);
	}
}

void ANNTreeRoot::init(const std::string& confPath, int slavePort) {
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
	// start slave processes
	std::string cmd = "parallel-ssh -h " + sConfPath
			+ " -t 0 \"ANNTreeDaemon -c " + sConfPath + " -p "
			+ boost::lexical_cast<std::string>(slavePort)
			+ " > /tmp/anntree_slave.log\" &";
	if (system(cmd.c_str()) != 0) {
		std::cerr << "Error in starting ANNTreeSlave." << std::endl;
		return;
	}
	sleep(3);
	// connect to slaves
	for (size_t i = 0; i < sSlaveNameArray.size(); ++i) {
		try {
			boost::shared_ptr<TSocket> socket(
					new TSocket(sSlaveNameArray[i], slavePort));
			boost::shared_ptr<TTransport> transport(
					new TFramedTransport(socket));
			boost::shared_ptr<TProtocol> protocol(
					new TBinaryProtocol(transport));
			sSlaveArray.push_back(
					boost::shared_ptr<ANNTreeDaemonClient>(
							new ANNTreeDaemonClient(protocol)));
			transport->open();
		} catch (const TException &tx) {
			std::cerr << tx.what() << std::endl;
		}
	}
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
	if (sampleCount > (int) sSlaveArray.size()) {
		// OpenCV kmeans has bug when clustering one point for one cluster
		cv::Mat clusters = cv::Mat(mMat.rows, 1, CV_32SC1);
		cv::kmeans(mMat, sSlaveArray.size(), clusters,
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
		if (dists[0] > 0.85 * dists[1] && indices[0] != indices[1]) {
			pSlaveArray->push_back(mLabelArray[indices[1]]);
		}
	}
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
		sSlaveArray[slaveArray[0]]->slaveAddFeature(mTreeIndex, id, strFeature);
	}
}

void ANNTreeRoot::index() {
	for (size_t i = 0; i < sSlaveArray.size(); ++i) {
		sSlaveArray[i]->slaveIndex(mTreeIndex);
	}
}

void ANNTreeRoot::knnSearch(std::vector<Neighbor>* pReturn,
		const std::vector<float>& feature, int k) {
	pReturn->clear();
	std::string strFeature;
	TypeConverter::readArrayToString(&strFeature, feature);
	std::vector<int> slaveArray;
	getSlave(&slaveArray, feature);
	if (slaveArray.size() == 1) {
		sSlaveArray[slaveArray[0]]->slaveKnnSearch(*pReturn, mTreeIndex,
				strFeature, k);
	}
	if (slaveArray.size() == 2) {
		std::vector<Neighbor> candidateArray1;
		std::vector<Neighbor> candidateArray2;
		sSlaveArray[slaveArray[0]]->slaveKnnSearch(candidateArray1, mTreeIndex,
				strFeature, k);
		sSlaveArray[slaveArray[1]]->slaveKnnSearch(candidateArray2, mTreeIndex,
				strFeature, k);
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
}
