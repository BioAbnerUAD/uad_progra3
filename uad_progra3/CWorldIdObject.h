#pragma once
#include "Include\C3DModel.h"
class CWorldIdObject
{
public:
	CWorldIdObject();
	CWorldIdObject(C3DModel * model);
	~CWorldIdObject();
private:
	C3DModel * m;
	const C3DModel * getModel() const;
	void setModel(C3DModel* m);
};

