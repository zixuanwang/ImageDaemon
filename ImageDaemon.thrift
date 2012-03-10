namespace cpp ImageDaemon
struct Bin{
	1:i64 visualwordID,
	2:double frequency
}

service ImageDaemon{
	list<Bin> getBoWFeature(1:i64 rowKey),
	void computeColorFeature(1:i64 rowKey),
	list<string> query(1:string imagePath),
	void addImage(1:string imageHash, 2:i64 rowKey),
	void indexImage(1:string imageHash, 2:i64 rowKey),
	void cropImage(1:string imagePath, 2:string cropImagePath, 3:i32 width, 4:i32 height)
}
