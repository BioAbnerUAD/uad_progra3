#pragma once
#include "CChunk.h"
#include "OctTree.h"
class CCubeGrid
{
public:
	CCubeGrid();
	~CCubeGrid();
	CChunk * getChunk(CVector3 pos);
	CChunk * getChunk(float x, float y, float z);
	void addChunk(CChunk* chunk);
	size_t chunksSize();
	OctTree<CChunk>* getChunks();
private:
	OctTree<CChunk> chunks;
};

