#pragma once
#include "Include\CVector3.h"
#include "CWorldIdObject.h"
class CWorldObjectInstance
{
public:
	CWorldObjectInstance();
	CWorldObjectInstance( CVector3 pos, CWorldIdObject &wObject);
	~CWorldObjectInstance();
	const size_t getWorldObjectID() const;
	void setWorldObjectID(size_t ID);
private:
	CVector3 pos;
	CWorldIdObject *wo;
	size_t worldObjectID;
};

