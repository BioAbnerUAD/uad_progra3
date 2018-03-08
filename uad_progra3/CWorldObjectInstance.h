#pragma once
#include "Include\CVector3.h"
#include "CWorldIdObject.h"
class CWorldObjectInstance
{
public:
	CWorldObjectInstance(CWorldIdObject *idObj, CVector3 pos);
	~CWorldObjectInstance();
	CVector3 pos;
	CWorldIdObject *wo;
	int worldObjectID;
};

