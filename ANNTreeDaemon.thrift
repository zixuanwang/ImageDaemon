namespace cpp ANNTreeDaemon

struct Neighbor{
	1:i64 id,
	2:double distance
}

service ANNTreeDaemon{
	list<Neighbor> knnSearch(1:string feature, 2:i32 k),
	void addFeature(1:i64 id, 2:string feature),
	void build(),
	void slaveIndex(),
	void slaveInit(),
	void slaveSave(),
	void slaveAddFeature(1:i64 id, 2:string feature),
	list<Neighbor> slaveKnnSearch(1:string feature, 2:i32 k),
	list<Neighbor> similarSearch(1:i64 id, 2:i32 k, 3:string type)
}
