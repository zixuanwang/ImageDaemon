/*
 * InvertedIndexClient.h
 *
 *  Created on: Mar 4, 2012
 *      Author: zixuan
 */

#ifndef INVERTEDINDEXCLIENT_H_
#define INVERTEDINDEXCLIENT_H_

#include "GlobalConfig.h"
#include "InvertedIndexDaemon.h"
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace  ::net::walnutvision;
class InvertedIndexClient {
public:
	InvertedIndexClient();
	~InvertedIndexClient();
	void loadPostingList(std::vector<Posting>* pPostingArray, int64_t visualwordId);
	void addPosting(int64_t visualwordId, const Posting& posting);
private:
	boost::shared_ptr<TSocket> mSocket;
	boost::shared_ptr<TTransport> mTransport;
	boost::shared_ptr<TProtocol> mProtocol;
	boost::shared_ptr<InvertedIndexDaemonClient> mClient;
};

#endif /* INVERTEDINDEXCLIENT_H_ */
