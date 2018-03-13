#pragma once
#include "CCell.h"
#include <vector>

// Constants based in minecraft
// https://minecraft.gamepedia.com/Chunk
// 64x64x256  blocks

#define CHUNK_SIZE 4//64 
#define CHUNK_HEIGHT 4//256
#define BLOCK_SIZE 1.0f

class CChunk
{
public:
	CChunk(vector<CWorldIdObject*>* idObjs, int x, int y);
	~CChunk();
	CCell blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_HEIGHT];
	int x, y;
	unsigned short *m_vertexIndices;

	CVector3 *m_vertices;
	float *m_verticesRaw;

	int m_numVertices, m_numFaces;
};

