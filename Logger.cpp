/*
 * Logger.cpp
 *
 *  Created on: Mar 1, 2012
 *      Author: zixuan
 */

#include "Logger.h"

Logger* Logger::pInstance = NULL;
log4cxx::LoggerPtr Logger::sLogger;

Logger* Logger::instance() {
	if (pInstance == NULL) {
		pInstance = new Logger;
		// Create pattern layout
		log4cxx::PatternLayoutPtr patternLayout(new log4cxx::PatternLayout());
		patternLayout->setConversionPattern(
				LOG4CXX_STR("%d{%Y-%m-%d %H:%M:%S} [%-5p] (%l:%c{5}): %m %n"));
		// Create console appender
		log4cxx::ConsoleAppenderPtr consoleAppender(new log4cxx::ConsoleAppender(patternLayout));
		// Get rootLogger add appenders
		sLogger = log4cxx::Logger::getRootLogger();
		sLogger->addAppender(consoleAppender);
	}
	return pInstance;
}

Logger::Logger() {

}

void Logger::log(const std::string& msg) {
	LOG4CXX_INFO(sLogger, msg);
}
