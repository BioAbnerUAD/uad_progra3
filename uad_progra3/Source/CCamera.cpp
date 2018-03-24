#include "../Include/CCamera.h"
#include "../CChunk.h"

CCamera::CCamera()
{
	initPosition.setValues((1 - CHUNK_SIZE) / 2.f, 0, (1 - CHUNK_SIZE) / 2.f);
	position = initPosition;
	up.setValues(0, 1, 0); //Vector pointing up
	lookAt.setValues(0, 0, 1); //Vector pointing to the front
}

void CCamera::Move(float dx, float dy)
{
	position.setValues(position.getX() + dx, position.getY(), position.getZ() + dy);
}

void CCamera::Reset()
{
	position = initPosition;
}
