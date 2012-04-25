/*
 * ScopedThriftClient.h
 *
 *  Created on: Apr 18, 2012
 *      Author: zixuan
 */

#ifndef SCOPEDTHRIFTCLIENT_H_
#define SCOPEDTHRIFTCLIENT_H_

#include "GlobalConfig.h"
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

class ScopedThriftClient {
public:
	ScopedThriftClient(const std::string& serverName, int serverPort);
	virtual ~ScopedThriftClient();
protected:
	boost::shared_ptr<TSocket> mSocket;
	boost::shared_ptr<TTransport> mTransport;
	boost::shared_ptr<TProtocol> mProtocol;
};

#endif /* SCOPEDTHRIFTCLIENT_H_ */
