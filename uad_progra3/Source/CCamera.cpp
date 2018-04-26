#include "../Include/CCamera.h"
#include "../CChunk.h"

#define PI 3.14159265f

CCamera::CCamera()
{
	initPosition.setValues(0.5f, 2.f, 0.5f);
	position = initPosition;
	rotation.setValues(0, 0, 0);
	up.setValues(0, 1, 0); //Vector pointing up
	lookAt.setValues(0, -0.2588190451, 0.96592582628); //Vector pointing to the front
}

void CCamera::Move(float dx, float dy, float dz)
{
	float sin = sinf(rotation.getX());
	float cos = cosf(rotation.getX());

	float x = dx * cos + dz * sin;
	float z = -dx * sin + dz * cos;

	position.setValues(position.getX() + x, position.getY() + dy, position.getZ() + z);
}

void CCamera::Reset()
{
	position = initPosition;
	rotation.setValues(0, 0, 0);
}

void CCamera::Rotate(float dx, float dy)
{
	float Y = rotation.getY() + dy;
	if (Y > 5 * PI / 16)
		Y = 5 * PI / 16;
	else if (Y < -9 * PI / 16)
		Y = -9 * PI / 16;
	rotation.setValues(rotation.getX() + dx, Y, position.getZ());

	lookAt.rotate(dx, dy);
	up.rotate(dx, dy);;
}
