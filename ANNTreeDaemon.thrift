namespace cpp net.walnutvision
namespace java net.walnutvision

struct Neighbor{
	1:i64 id,
	2:double distance
}

service ANNTreeDaemon{
	void clear(),
	i32 addTree(),
	void loadSample(1:i32 treeIndex, 2:string sampleArray, 3:i32 sampleCount),
	void addFeature(1:i32 treeIndex, 2:i64 id, 3:string feature),
	void index(1:i32 treeIndex),
	list<Neighbor> knnSearch(1:i32 treeIndex, 2:string feature, 3:i32 k),
	list<Neighbor> similarSearch(1:i32 treeIndex, 2:i64 id, 3:i32 k),
	void slaveClear(),
	i32 slaveAddTree(),
	void slaveInit(1:i32 treeIndex),
	void slaveAddFeature(1:i32 treeIndex, 2:i64 id, 3:string feature),
	void slaveIndex(1:i32 treeIndex),
	list<Neighbor> slaveKnnSearch(1:i32 treeIndex, 2:string feature, 3:i32 k)
}
