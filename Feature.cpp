/*
 * Feature.cpp
 *
 *  Created on: Jan 21, 2012
 *      Author: zixuan
 */

#include "Feature.h"

Feature::Feature() {

}

Feature::~Feature() {

}

void Feature::save(std::string* pString) {
	int length = size() * sizeof(float);
	pString->assign(length, 0);
	if (length != 0) {
		memcpy((char*) &(*pString)[0], (char*) &mVector[0], length);
	}
}

void Feature::load(const std::string& string) {
	int size = (int) string.size() / sizeof(float);
	mVector.assign(size, 0.0);
	if (size != 0) {
		memcpy((char*) &mVector[0], (char*) &string[0], string.size());
	}
}

int Feature::size() {
	return (int) mVector.size();
}
