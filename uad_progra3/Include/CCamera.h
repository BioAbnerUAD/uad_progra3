#pragma once
#include "CVector3.h"

#define DEFAULT_MOVEMENT_SPEED 0.5

class CCamera
{
public:
	CCamera();
	~CCamera() {};
	CVector3 initPosition;
	CVector3 position;
	CVector3 lookAt;
	CVector3 up;

	void Move(float dx, float dy);
	void Reset();
};

