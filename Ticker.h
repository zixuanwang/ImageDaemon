/*
 * Ticker.h
 *
 *  Created on: Jan 29, 2012
 *      Author: zixuan
 */

#ifndef TICKER_H_
#define TICKER_H_

#include "GlobalConfig.h"
class Ticker {
public:
	Ticker();
	~Ticker();
	void start() {
		mTStart = boost::posix_time::microsec_clock::local_time();
	}
	void stop() {
		mTStop = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::time_duration diff = mTStop - mTStart;
		std::cout << diff.total_milliseconds() << " ms has elapsed" << std::endl;
	}
private:
	boost::posix_time::ptime mTStart;
	boost::posix_time::ptime mTStop;
};

#endif /* TICKER_H_ */
