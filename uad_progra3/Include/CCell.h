#pragma once
#include "CWorldObjectInstance.h"
#include "CWorldIdObject.h"
class CCell
{
public:
	CCell();
	void initialize(CVector3& centro, CWorldIdObject* idObj);
	~CCell();
	CWorldObjectInstance* instance;
	CVector3 centro;
};

