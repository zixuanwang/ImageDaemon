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
	list<Bin> getBoWFeature(1:i64 rowKey),
	void addPostingList(1:i64 visualwordID, 2:list<Posting> postingArray),
	void addImage(1:string imageHash, 2:i64 imageKey),
	void computeColorFeature(1:i64 rowKey),
	list<string> query(1:string imagePath),
	void loadInvertedIndex()
}
