/*
 * ScopedANNTreeDaemonClient.cpp
 *
 *  Created on: Apr 18, 2012
 *      Author: zixuan
 */

#include "ScopedANNTreeDaemonClient.h"

ScopedANNTreeDaemonClient::ScopedANNTreeDaemonClient(
		const std::string& serverName, int serverPort) :
		ScopedThriftClient(serverName, serverPort) {
	client.reset(new ANNTreeDaemonClient(mProtocol));
	try {
		mTransport->open();
	} catch (const TException& tx) {
		std::cerr << tx.what() << std::endl;
	}
}

ScopedANNTreeDaemonClient::~ScopedANNTreeDaemonClient() {
}

