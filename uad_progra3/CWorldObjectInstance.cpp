#include "CWorldObjectInstance.h"



CWorldObjectInstance::CWorldObjectInstance(CWorldIdObject *idObj, CVector3 pos):pos(pos)
{
	worldObjectID = idObj->ID;
	wo = idObj;
}

CWorldObjectInstance::~CWorldObjectInstance()
{
}
