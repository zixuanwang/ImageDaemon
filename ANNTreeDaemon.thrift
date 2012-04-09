namespace cpp net.walnutvision
namespace java net.walnutvision

struct Neighbor{
	1:i64 id,
	2:double distance
}

service ANNTreeDaemon{
	void clear(),
	void putTree(1:i32 treeIndex),
	void loadSample(1:i32 treeIndex, 2:string sampleArray, 3:i32 sampleCount),
	void addFeature(1:i32 treeIndex, 2:i64 id, 3:string feature),
	void index(1:i32 treeIndex),
	list<Neighbor> knnSearch(1:i32 treeIndex, 2:string feature, 3:i32 k),
	list<Neighbor> similarSearch(1:i32 treeIndex, 2:i64 id, 3:i32 k),
	void slavePutTree(1:i32 treeIndex),
	void slaveAddFeature(1:i32 treeIndex, 2:i64 id, 3:string feature),
	void slaveIndex(1:i32 treeIndex),
	list<Neighbor> slaveKnnSearch(1:i32 treeIndex, 2:string feature, 3:i32 k),
	void buildCategory(1:string categoryName),
	void buildAllCategory(),
	list<string> query(1:string imagePath, 2:i32 treeIndex, 3:string featureType, 4:i32 k)
}
