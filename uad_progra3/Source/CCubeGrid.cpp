#include "../Include/CCubeGrid.h"
#include <string>

CCubeGrid::CCubeGrid()
{
}


CCubeGrid::~CCubeGrid()
{
}

CChunk * CCubeGrid::getChunk(int x, int y)
{
	return chunks[std::to_string(x) + "," + std::to_string(y)];
}

void CCubeGrid::addChunk(CChunk * chunk)
{
	chunks.insert(std::pair<std::string, CChunk*>(std::to_string(chunk->x) + "," + std::to_string(chunk->y), chunk));
}

size_t CCubeGrid::chunksSize()
{
	return chunks.size();
}

const std::map<std::string, CChunk*>* CCubeGrid::getChunks() const
{
	return &chunks;
}
