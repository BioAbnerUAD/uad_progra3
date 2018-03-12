#pragma once
#include "CChunk.h"
#include <map>
class CCubeGrid
{
public:
	CCubeGrid();
	~CCubeGrid();
	CChunk * getChunk(int x, int y);
	void addChunk(CChunk* chunk);
	size_t chunksSize();
	const map<string, CChunk*>* getChunks() const;
private:
	map<string, CChunk*> chunks;
};
