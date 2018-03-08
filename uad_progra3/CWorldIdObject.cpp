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
}


CWorldIdObject::~CWorldIdObject()
{
}
