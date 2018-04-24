#include "../Include/CCell.h"



CCell::CCell()
{
	
}

void CCell::initialize(CVector3& centro, CWorldIdObject * idObj)
{
	this->centro = centro;
	this->instance = new CWorldObjectInstance(idObj, centro);
}


CCell::~CCell()
{
}
