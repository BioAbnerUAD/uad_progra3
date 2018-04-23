#pragma once
#include "CCell.h"
#include <vector>

#define CHUNK_SIZE 8		//64 
#define CHUNK_HEIGHT 2		//256
#define BLOCK_SIZE 1.0f

class CChunk
{
public:
	CChunk(CWorldIdObject* idObj, int x, int y, int z);
	~CChunk();
	CCell blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
	int x, y, z;

	unsigned int textureID = -1;
	unsigned int shaderProgramID = 0;
	unsigned int VAOID = 0;

	float* getVertices() { return m_verticesRaw; }
	float* getUVs() { return m_uvsRaw;  }
	size_t getNumVertices() { return m_numVertices; }
	unsigned short* getVertexIndices() { return m_vertexIndices; }
	size_t getNumFaces() { return m_numFaces; }
private:

	unsigned short *m_vertexIndices;

	CVector3 *m_vertices;
	float *m_verticesRaw;
	CVector3 *m_uvs;
	float *m_uvsRaw;

	int m_numVertices, m_numFaces;
};

