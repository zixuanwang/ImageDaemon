/*
 * InvertedIndex.h
 *
 *  Created on: Jan 9, 2012
 *      Author: zixuan
 */

#ifndef INVERTEDINDEX_H_
#define INVERTEDINDEX_H_
#include "GlobalConfig.h"
#include "HbaseAdapter.h"
#include "MongoDBAdapter.h"
#include "ImageDaemon_types.h"

using namespace ImageDaemon;
class InvertedIndex {
public:
	static InvertedIndex* instance();
	void savePostingList(int64_t visualwordID,
			const std::vector<Posting>& postingArray);
	void loadPostingList(std::vector<int64_t>* pImageIDArray, std::vector<double>* pImageScoreArray,
			int64_t visualwordID);
private:
	static InvertedIndex* pInstance;
	// Default constuctor, copy constructor and assignment operators should be as private so that nobody
	// from outside can call those functions and instantiate it
	InvertedIndex();
	InvertedIndex(const InvertedIndex&);
	InvertedIndex& operator=(const InvertedIndex&);
	struct PostingComparator {
		bool operator()(const Posting &i, const Posting &j) {
			return (i.score < j.score);
		}
	};
};

#endif /* INVERTEDINDEX_H_ */
