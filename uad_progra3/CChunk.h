#pragma once
#include "CWorldObjectInstance.h"

// Constants based in minecraft
// https://minecraft.gamepedia.com/Chunk
// 64x64x256  blocks

#define CHUNK_SIZE 64 
#define CHUNK_HEIGHT 256
#define BLOCK_SIZE 1.0f

class CChunk
{
public:
	CChunk();
	~CChunk();
	CWorldObjectInstance blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_HEIGHT];
};

