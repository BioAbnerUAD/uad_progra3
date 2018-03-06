#include "CWorldIdObject.h"



CWorldIdObject::CWorldIdObject()
{
}

CWorldIdObject::CWorldIdObject(C3DModel * model):m(model){}


CWorldIdObject::~CWorldIdObject()
{
}

const C3DModel * CWorldIdObject::getModel() const
{
	return m;
}

void CWorldIdObject::setModel(C3DModel * m)
{
	this->m = m;
}
