/*
 * TypeConverter.h
 *
 *  Created on: Jan 6, 2012
 *      Author: zixuan
 */

#ifndef TYPECONVERTER_H_
#define TYPECONVERTER_H_
#include "GlobalConfig.h"
class TypeConverter {
public:
	TypeConverter();
	~TypeConverter();
	template<typename T> static void readNumToString(std::string* pString,
			T number) {
		int size = sizeof(T);
		char* ptr = (char*) &number;
		pString->assign(ptr, ptr + size);
	}
	template<typename T> static void readStringToNum(T* number,
			const std::string& string) {
		memcpy((char*) number, (char*) &string[0], sizeof(T));
	}
	template<typename T> static void readArrayToString(std::string* pString,
			const std::vector<T> array) {
		int size = (int) array.size() * sizeof(T);
		pString->assign(size, 0);
		if (size > 0) {
			memcpy((char*) &pString[0], (char*) &array[0], size);
		}
	}
	template<typename T> static void readStringToArray(std::vector<T>* pArray,
			const std::string& string) {
		int size = (int) string.size() / sizeof(T);
		pArray->assign(size, 0);
		if (size > 0) {
			memcpy((char*) &pArray[0], (char*) &string[0], string.size());
		}
	}
};

#endif /* TYPECONVERTER_H_ */
