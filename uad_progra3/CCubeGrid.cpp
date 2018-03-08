#include "CCubeGrid.h"



CCubeGrid::CCubeGrid()
{
}


CCubeGrid::~CCubeGrid()
{
}

CChunk * CCubeGrid::getChunk(int x, int y)
{
	return chunks[to_string(x) + "," + to_string(y)];
}

void CCubeGrid::addChunk(CChunk * chunk)
{
	chunks.insert(pair<string, CChunk*>(to_string(chunk->x) + "," + to_string(chunk->y), chunk));
}

size_t CCubeGrid::chunksSize()
{
	return chunks.size();
}

const map<string, CChunk*>* CCubeGrid::getChunks() const
{
	return &chunks;
}
