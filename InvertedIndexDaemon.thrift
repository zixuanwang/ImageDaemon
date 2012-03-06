namespace cpp InvertedIndexDaemon

struct Posting{
	1:i64 imageId,
	2:double score
}

service InvertedIndexDaemon{
	void init(),
	void savePostingList(1:i64 visualwordId, 2:list<Posting> postingArray),
	list<Posting> loadPostingList(1:i64 visualwordId),
	void addPosting(1:i64 visualwordId, 2:Posting posting)
}
