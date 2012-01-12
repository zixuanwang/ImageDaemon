/*
 * TypeConverter.h
 *
 *  Created on: Jan 6, 2012
 *      Author: zixuan
 */

#ifndef TYPECONVERTER_H_
#define TYPECONVERTER_H_
#include <string>
#include <cstring>
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
		memcpy((char*) number, (char*)&string[0], sizeof(T));
	}
};

#endif /* TYPECONVERTER_H_ */
