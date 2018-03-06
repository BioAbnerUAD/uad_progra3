#include "CWorldObjectInstance.h"



CWorldObjectInstance::CWorldObjectInstance()
{
}


CWorldObjectInstance::CWorldObjectInstance(CVector3 pos, CWorldIdObject &wObject): pos(pos),wo(&wObject){}

CWorldObjectInstance::~CWorldObjectInstance()
{
}

const size_t CWorldObjectInstance::getWorldObjectID() const
{
	return worldObjectID;
}

void CWorldObjectInstance::setWorldObjectID(size_t ID)
{
	worldObjectID = ID;
}
