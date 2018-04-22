#pragma once
#include "../Include/C3DModel.h"
class CWorldIdObject
{
public:
	CWorldIdObject(int ID);
	~CWorldIdObject();
	int ID;
	CVector3 m_vertices[8];
	float m_verticesRaw[24];
	unsigned short m_indices[36];
	int m_numFaces;
};
