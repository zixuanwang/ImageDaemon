/*
 * ScopedThriftClient.cpp
 *
 *  Created on: Apr 18, 2012
 *      Author: zixuan
 */

#include "ScopedThriftClient.h"

ScopedThriftClient::ScopedThriftClient(const std::string& serverName,
		int serverPort) {
	mSocket.reset(new TSocket(serverName, serverPort));
	mTransport.reset(new TFramedTransport(mSocket));
	mProtocol.reset(new TBinaryProtocol(mTransport));
}

ScopedThriftClient::~ScopedThriftClient() {
	mTransport->close();
}

