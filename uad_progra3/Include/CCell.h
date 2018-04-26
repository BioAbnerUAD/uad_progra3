#pragma once
#include "CWorldObjectInstance.h"
#include "CWorldIdObject.h"
class CCell
{
public:
	CCell(CVector3& centro, CWorldIdObject* idObj);
	~CCell();
	CWorldObjectInstance* instance;
	CVector3 centro;
};

