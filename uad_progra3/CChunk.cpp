#include "CChunk.h"

CChunk::CChunk(vector<CWorldIdObject*>* idObjs, int x, int y):x(x),y(y)
{
	indicesTrigs.clear();
	trigsRaw.clear();

	size_t offset = 0;

	CWorldIdObject* idObj = idObjs->operator[](0);

	for (size_t i = 0; i < CHUNK_SIZE; i++)
		for (size_t j = 0; j < CHUNK_SIZE; j++)
			for (size_t k = 0; k < CHUNK_HEIGHT; k++)
			{
				CVector3 centro(i, j, k);
				blocks[i][j][k].initialize(centro, idObj);

				for (size_t u = 0; u < 8; u++)
				{
					trigsRaw.push_back(blocks[i][j][k].vertex[u].getX());
					trigsRaw.push_back(blocks[i][j][k].vertex[u].getY());
					trigsRaw.push_back(blocks[i][j][k].vertex[u].getZ());
				}

				//Inicialisar Triangulos
				for (size_t v = 0; v < 12; v++)
				{	
					indicesTrigs.push_back(offset + idObj->m_indices[v * 3]);
					indicesTrigs.push_back(offset + idObj->m_indices[v * 3 + 1]);
					indicesTrigs.push_back(offset + idObj->m_indices[v * 3 + 2]);
				}

				offset += 36;
			}
}


CChunk::~CChunk()
{
}
