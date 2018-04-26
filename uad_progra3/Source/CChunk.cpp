#include "../Include/CChunk.h"
#include <iostream>

CChunk::CChunk(CWorldIdObject* idObj, int x, int y, int z)
	:x(x * CHUNK_SIZE),y(y * CHUNK_HEIGHT),z(z * CHUNK_SIZE)
{
	for (int i = 0; i < CHUNK_SIZE; i++)
		for (int j = 0; j < CHUNK_HEIGHT; j++)
			for (int k = 0; k < CHUNK_SIZE; k++)
			{
				CVector3 centro((float)(i + this->x), (float)(-j + this->y), (float)(k + this->z));
				//blocks[i][j][k].initialize(centro, idObj);
			}
}


CChunk::~CChunk()
{
}
