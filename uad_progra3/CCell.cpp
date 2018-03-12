#include "CCell.h"



CCell::CCell()
{
	
}

void CCell::initialize(CVector3& centro, CWorldIdObject * idObj)
{
	this->centro = centro;
	this->instance = new CWorldObjectInstance(idObj, centro);
	for (size_t i = 0; i < 8; i++)
	{
		vertex[i] = centro + idObj->m_vertices[i];
	}
}


CCell::~CCell()
{
}
