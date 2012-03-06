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
#include "InvertedIndexDaemon_types.h"

using namespace ::InvertedIndexDaemon;
class InvertedIndex {
public:
	static InvertedIndex* instance();
	void init();
	/// This function is called by the reducer
	void savePostingList(int64_t visualwordId,
			const std::vector<Posting>& postingArray);
	void loadPostingList(std::vector<Posting>* pPostingArray, int64_t visualwordId);
	void addPosting(int64_t visualwordId, const Posting& posting);
private:
	/// This function updates the champion list in memory
	void updateChampionList(int64_t visualwordId, const std::vector<Posting>& postingArray);
	void savePosting(const std::string& strVisualwordId, const std::string& columnName, const Posting& posting);
	static InvertedIndex* pInstance;
	InvertedIndex();
	InvertedIndex(const InvertedIndex&);
	InvertedIndex& operator=(const InvertedIndex&);
	struct PostingComparator {
		bool operator()(const Posting &i, const Posting &j) {
			return (i.score < j.score);
		}
	};

	std::vector<std::multiset<Posting, PostingComparator> > mInvertIndex;
	static pthread_mutex_t sMutex;
};

#endif /* INVERTEDINDEX_H_ */
