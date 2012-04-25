/*
 * File.cpp
 *
 *  Created on: Apr 18, 2012
 *      Author: zixuan
 */

#include "File.h"

File::File() {

}

File::~File() {

}

void File::getFiles(std::vector<std::string>* pFileArray,
		const std::string& directory, bool recursive) {
	if (!boost::filesystem::exists(directory)) {
		return;
	}
	boost::filesystem::directory_iterator end_itr;
	for (boost::filesystem::directory_iterator itr(directory); itr != end_itr;
			++itr) {
		if (recursive && boost::filesystem::is_directory(itr->status())) {
			getFiles(pFileArray, itr->path().string(), recursive);
		} else {
			pFileArray->push_back(itr->path().string());
		}
	}
}
