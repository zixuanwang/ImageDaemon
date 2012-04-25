namespace cpp net.walnutvision
namespace java net.walnutvision
struct Bin{
	1:i64 visualwordID,
	2:double frequency
}

service ImageDaemon{
	list<Bin> computeBoWFeature(1:i64 rowKey),
	void computeColorFeature(1:i64 rowKey),
	void computeShapeFeature(1:i64 rowKey),
	void computeSURFFeature(1:i64 rowKey),
	list<string> query(1:string imagePath),
	void indexImage(1:string imageHash, 2:i64 rowKey),
	void addImage(1:string imageHash, 2:i64 rowKey),
	void cropImage(1:i64 rowKey, 2:i32 width, 3:i32 height),
	void cropUploadImage(1:string srcPath, 2:string dstPath, 3:i32 width, 4:i32 height),
	void segment(1:i64 rowKey)
}
