#include "../Include/CCamera.h"

CCamera::CCamera()
{
	initPosition.setValues(0, 0, 0);
	position.setValues(0, 0, 0);
	up.setValues(0, 1, 0); //Vector pointing up
	lookAt.setValues(0, 0, 1); //Vector pointing to the front
}

void CCamera::Move(float dx, float dy)
{
	position.setValues(position.getX() + dx, position.getY() + dy, position.getZ());
}

void CCamera::Reset()
{
	position = initPosition;
}