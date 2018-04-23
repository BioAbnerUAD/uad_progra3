#pragma once
#include "../Include/C3DModel.h"
class CWorldIdObject
{
public:
	CWorldIdObject(int ID);
	~CWorldIdObject();
	int ID;
	CVector3 m_vertices[14];
	CVector3 m_UVs[14];
	unsigned short m_indices[36];
	size_t m_numFaces;
	size_t m_numVertices;
};

