#include "../Include/CChunk.h"
#include <iostream>

CChunk::CChunk(CWorldIdObject* idObj, int x, int y, int z)
	:x(x * CHUNK_SIZE),y(y * CHUNK_HEIGHT),z(z * CHUNK_SIZE)
{
	std::vector<unsigned short> indices(0);
	std::vector<float> verticesR(0);
	std::vector<CVector3> vertices(0);
	std::vector<float> uvsR(0);
	std::vector<CVector3> uvs(0);

	unsigned short offset = 0;

	for (int i = 0; i < CHUNK_SIZE; i++)
		for (int j = 0; j < CHUNK_HEIGHT; j++)
			for (int k = 0; k < CHUNK_SIZE; k++)
			{
				CVector3 centro((float)(i + this->x), (float)(-j + this->y), (float)(k + this->z));
				blocks[i][j][k].initialize(centro, idObj);

				// Inicializar Vertices
				for (size_t u = 0; u < idObj->m_numVertices; u++)
				{
					CVector3* vert = &idObj->m_vertices[u];
					
					vertices.push_back(*vert);

					verticesR.push_back(vert->getX());
					verticesR.push_back(vert->getY());
					verticesR.push_back(vert->getZ());
				}

				// Inicialisar Triangulos
				for (size_t w = 0; w < idObj->m_numVertices; w++)
				{
					CVector3* uv = &idObj->m_UVs[w];

					uvs.push_back(*uv);

					uvsR.push_back(uv->getX());
					uvsR.push_back(uv->getY());
				}

				// Inicialisar Triangulos
				for (size_t v = 0; v < idObj->m_numFaces * 3; v++)
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

	this->m_uvs = new CVector3[uvs.size()];
	memcpy(this->m_uvs, &uvs[0], uvs.size() * sizeof(CVector3));

	this->m_uvsRaw = new float[uvsR.size()];
	memcpy(this->m_uvsRaw, &uvsR[0], uvsR.size() * sizeof(float));

	this->m_numFaces = (int)indices.size() / 3;
	this->m_numVertices = (int)vertices.size();
}


CChunk::~CChunk()
{
}
