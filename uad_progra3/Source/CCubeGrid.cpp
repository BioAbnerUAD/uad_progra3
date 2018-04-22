#include "../Include/CCubeGrid.h"
#include <string>

CCubeGrid::CCubeGrid() : chunks(Zone(CVector3(0, 0, 0),
	CVector3(CHUNK_SIZE * 64, CHUNK_HEIGHT * 8, CHUNK_SIZE * 64)))
{
}


CCubeGrid::~CCubeGrid()
{
}

CChunk * CCubeGrid::getChunk(CVector3 pos)
{
	return chunks.GetIndex(pos);
}

CChunk * CCubeGrid::getChunk(float x, float y, float z)
{
	CVector3 pos(x, y, z);
	return chunks.GetIndex(pos);
}

void CCubeGrid::addChunk(CChunk * chunk)
{
	chunks.Insert(chunk, CVector3(
		(float)chunk->x, (float)chunk->y, (float)chunk->z));
}

size_t CCubeGrid::chunksSize()
{
	return chunks.size();
}

OctTree<CChunk>* CCubeGrid::getChunks()
{
	return &chunks;
}
