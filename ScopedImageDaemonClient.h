/*
 * ScopedImageDaemonClient.h
 *
 *  Created on: Apr 19, 2012
 *      Author: zixuan
 */

#ifndef SCOPEDIMAGEDAEMONCLIENT_H_
#define SCOPEDIMAGEDAEMONCLIENT_H_
#include "ScopedThriftClient.h"
#include "ImageDaemon.h"
using namespace ::net::walnutvision;
class ScopedImageDaemonClient: public ScopedThriftClient {
public:
	ScopedImageDaemonClient(const std::string& serverName, int serverPort);
	virtual ~ScopedImageDaemonClient();
	boost::shared_ptr<ImageDaemonClient> client;
};

#endif /* SCOPEDIMAGEDAEMONCLIENT_H_ */
