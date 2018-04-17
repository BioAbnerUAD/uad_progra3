#include "../Include/CChunk.h"
#include <iostream>

CChunk::CChunk(std::vector<CWorldIdObject*>* idObjs, int x, int y):x(x),y(y)
{
	std::vector<unsigned short> indices(0);
	std::vector<float> verticesR(0);
	std::vector<CVector3> vertices(0);

	size_t offset = 0;

	CWorldIdObject* idObj = idObjs->operator[](0);

	for (size_t i = 0; i < CHUNK_SIZE; i++)
		for (int j = 0; j < CHUNK_HEIGHT; j++)
			for (size_t k = 0; k < CHUNK_SIZE; k++)
			{
				CVector3 centro(i, -j, k);
				blocks[i][j][k].initialize(centro, idObj);

				for (size_t u = 0; u < 8; u++)
				{
					CVector3* vert = &blocks[i][j][k].vertex[u];
					
					vertices.push_back(*vert);

					verticesR.push_back(vert->getX());
					verticesR.push_back(vert->getY());
					verticesR.push_back(vert->getZ());
				}

				//Inicialisar Triangulos
				for (size_t v = 0; v < 36; v++)
				{	
					indices.push_back(offset + idObj->m_indices[v]);
				}

				offset += 8;
			}

	this->m_vertexIndices = new unsigned short[indices.size()];
	memcpy(this->m_vertexIndices, &indices[0], indices.size() * sizeof(unsigned short));

	this->m_vertices = new CVector3[vertices.size()];
	memcpy(this->m_vertices, &vertices[0], vertices.size() * sizeof(CVector3));

	this->m_verticesRaw = new float[verticesR.size()];
	memcpy(this->m_verticesRaw, &verticesR[0], verticesR.size() * sizeof(float));

	this->m_numFaces = indices.size() / 3;
	this->m_numVertices = vertices.size();
}


CChunk::~CChunk()
{
}
