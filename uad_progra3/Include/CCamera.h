#pragma once
#include "CVector3.h"

class CCamera
{
public:
	CCamera();
	~CCamera() {};

	void Move(float dx, float dy, float dz);
	void Reset();
	void Rotate(float dx, float dy);
	CVector3 getPosition() const { return position; }
	CVector3 getRotation() const { return rotation; }
	CVector3 getLookAt() const { return lookAt; }
	CVector3 getUp() const { return up; }

private:
	CVector3 initPosition;
	CVector3 position;
	CVector3 rotation;
	CVector3 lookAt;
	CVector3 up;
};

