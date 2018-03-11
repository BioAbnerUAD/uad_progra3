#include "CWorldIdObject.h"



CWorldIdObject::CWorldIdObject(int ID)
{
	this->ID = ID;

	m_vertices[0].setValues(0.5, 0.5, 0.5);
	m_vertices[1].setValues(0.5, 0.5, -0.5);
	m_vertices[2].setValues(0.5, -0.5, 0.5);
	m_vertices[3].setValues(0.5, -0.5, -0.5);
	m_vertices[4].setValues(-0.5, 0.5, 0.5);
	m_vertices[5].setValues(-0.5, 0.5, -0.5);
	m_vertices[6].setValues(-0.5, -0.5, 0.5);
	m_vertices[7].setValues(-0.5, -0.5, -0.5);

	for (size_t i = 0; i < 8; i++)
	{
		m_verticesRaw[i * 3] = m_vertices[0].getX();
		m_verticesRaw[i * 3 + 1] = m_vertices[0].getY();
		m_verticesRaw[i * 3 + 2] = m_vertices[0].getZ();
	};

	m_numFaces = 12;

	int indices[] = {
		0, 1, 2,
		1, 3, 2,
		4, 0, 6,
		0, 2, 6,
		5, 1, 4,
		1, 0, 4,
		5, 4, 7,
		4, 6, 7,
		6, 2, 7,
		2, 3, 7,
		7, 3, 5,
		3, 1, 5
	};

	for (size_t i = 0; i < 36; i++)
	{
		m_indices[i] = indices[i];
	}
}


CWorldIdObject::~CWorldIdObject()
{
}
