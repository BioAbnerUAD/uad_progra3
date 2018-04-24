#pragma once
#include "CCell.h"
#include <vector>

#define CHUNK_SIZE 100		//64 
#define CHUNK_HEIGHT 2		//256
#define BLOCK_SIZE 1.0f

class CChunk
{
public:
	CChunk(CWorldIdObject* idObj, int x, int y, int z);
	~CChunk();
	CCell blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
	int x, y, z;
};

