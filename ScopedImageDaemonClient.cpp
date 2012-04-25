/*
 * ScopedImageDaemonClient.cpp
 *
 *  Created on: Apr 19, 2012
 *      Author: zixuan
 */

#include "ScopedImageDaemonClient.h"

ScopedImageDaemonClient::ScopedImageDaemonClient(const std::string& serverName,
		int serverPort) :
		ScopedThriftClient(serverName, serverPort) {
	client.reset(new ImageDaemonClient(mProtocol));
	try {
		mTransport->open();
	} catch (const TException& tx) {
		std::cerr << tx.what() << std::endl;
	}
}

ScopedImageDaemonClient::~ScopedImageDaemonClient() {
}

