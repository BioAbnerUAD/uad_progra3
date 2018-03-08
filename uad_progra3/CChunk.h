#pragma once
#include "CCell.h"
#include <vector>

// Constants based in minecraft
// https://minecraft.gamepedia.com/Chunk
// 64x64x256  blocks

#define CHUNK_SIZE 64 
#define CHUNK_HEIGHT 256
#define BLOCK_SIZE 1.0f

class CChunk
{
public:
	CChunk(vector<CWorldIdObject*>* idObjs, int x, int y);
	~CChunk();
	CCell blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_HEIGHT];
	int x, y;
};

