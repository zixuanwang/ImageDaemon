/*
 * ScopedANNTreeDaemonClient.h
 *
 *  Created on: Apr 18, 2012
 *      Author: zixuan
 */

#ifndef SCOPEDANNTREEDAEMONCLIENT_H_
#define SCOPEDANNTREEDAEMONCLIENT_H_
#include "ScopedThriftClient.h"
#include "ANNTreeDaemon.h"
using namespace ::net::walnutvision;
class ScopedANNTreeDaemonClient: public ScopedThriftClient {
public:
	ScopedANNTreeDaemonClient(const std::string& serverName, int serverPort);
	virtual ~ScopedANNTreeDaemonClient();
	boost::shared_ptr<ANNTreeDaemonClient> client;
};

#endif /* SCOPEDANNTREEDAEMONCLIENT_H_ */
