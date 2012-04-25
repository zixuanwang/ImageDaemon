/*
 * File.h
 *
 *  Created on: Apr 18, 2012
 *      Author: zixuan
 */

#ifndef FILE_H_
#define FILE_H_
#include "GlobalConfig.h"

class File {
public:
	File();
	~File();
	static void getFiles(std::vector<std::string>* pFileArray,
			const std::string& directory, bool recursive = false);
};

#endif /* FILE_H_ */
