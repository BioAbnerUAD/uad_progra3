#include "CChunk.h"

CChunk::CChunk()
{
	for (size_t i = 0; i < CHUNK_SIZE; i++)
		for (size_t j = 0; j < CHUNK_SIZE; j++)
			for (size_t k = 0; k < CHUNK_HEIGHT; k++)
			{
				blocks[i][j][k].setWorldObjectID(0);
			}
}


CChunk::~CChunk()
{
}
