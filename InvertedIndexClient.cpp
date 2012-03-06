/*
 * InvertedIndexClient.cpp
 *
 *  Created on: Mar 4, 2012
 *      Author: zixuan
 */

#include "InvertedIndexClient.h"

InvertedIndexClient::InvertedIndexClient() {
	try {
		mSocket.reset(new TSocket(GlobalConfig::INVERTED_INDEX_SERVER, 9991));
		mTransport.reset(new TFramedTransport(mSocket));
		mProtocol.reset(new TBinaryProtocol(mTransport));
		mClient.reset(new InvertedIndexDaemonClient(mProtocol));
		mTransport->open();
	} catch (const TException &tx) {
		std::cerr << "Thrift openning error " << tx.what() << std::endl;
	}
}

InvertedIndexClient::~InvertedIndexClient() {
	mTransport->close();
}

void InvertedIndexClient::loadPostingList(std::vector<Posting>* pPostingArray,
		int64_t visualwordId) {
	mClient->loadPostingList(*pPostingArray, visualwordId);
}

void InvertedIndexClient::addPosting(int64_t visualwordId,
		const Posting& posting) {
	mClient->addPosting(visualwordId, posting);
}
