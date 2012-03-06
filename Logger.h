/*
 * Logger.h
 *
 *  Created on: Mar 1, 2012
 *      Author: zixuan
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include "GlobalConfig.h"

class Logger {
public:
	static Logger* instance();
	void log(const std::string& msg);
private:
	Logger();
	Logger(const Logger&);
	Logger& operator=(const Logger&);
	static Logger* pInstance;
	static log4cxx::LoggerPtr sLogger;
};

#endif /* LOGGER_H_ */
