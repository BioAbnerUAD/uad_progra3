#include "CChunk.h"

CChunk::CChunk(vector<CWorldIdObject*>* idObjs, int x, int y):x(x),y(y)
{
	for (size_t i = 0; i < CHUNK_SIZE; i++)
	{
		for (size_t j = 0; j < CHUNK_SIZE; j++)
		{
			for (size_t k = 0; k < CHUNK_HEIGHT; k++)
			{
				CVector3 centro(i, j, k);
				blocks[i][j][k].initialize(centro, idObjs->operator[](0));
			}
		}
	}
}


CChunk::~CChunk()
{
}
