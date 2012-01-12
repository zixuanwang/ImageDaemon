namespace cpp ImageDaemon
struct Bin{
	1:i64 visualwordID,
	2:double frequency
}

struct Posting{
	1:i64 imageID,
	2:double score
}

service ImageDaemon{
	list<Bin> getBoWFeatureFromHbase(1:i64 rowKey),
	void addPostingList(1:i64 visualwordID, 2:list<Posting> postingArray)
}
