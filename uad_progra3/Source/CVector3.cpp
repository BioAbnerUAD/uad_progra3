#include "../stdafx.h"

#include "../Include/CVector3.h"
#include <math.h>

/*
*/
CVector3::CVector3() 
	: m_X(0.0f), 
	m_Y(0.0f), 
	m_Z(0.0f) 
{
}

/*
*/
CVector3::CVector3(float _x, float _y, float _z)
	: m_X(_x), m_Y(_y), m_Z(_z)
{
}

/*
*/
CVector3::CVector3(const CVector3 &other)
{
	m_X = other.getX();
	m_Y = other.getY();
	m_Z = other.getZ();
}

/*
*/
CVector3& CVector3::operator=(const CVector3 &other)
{
	m_X = other.getX();
	m_Y = other.getY();
	m_Z = other.getZ();

	return *this;
}

/*
*/
CVector3& CVector3::operator+=(const CVector3 &other)
{
	m_X += other.getX();
	m_Y += other.getY();
	m_Z += other.getZ();

	return *this;
}

CVector3 CVector3::operator+(const CVector3 & other)
{
	CVector3 res;
	res.m_X = m_X + other.getX();
	res.m_Y = m_Y + other.getY();
	res.m_Z = m_Z + other.getZ();
	return res;
}

CVector3 CVector3::cross(const CVector3 & other)
{
	CVector3 res;
	res.setValues(
		this->getY()*other.getZ() - this->getZ()*other.getY(),
		this->getZ()*other.getX() - this->getX()*other.getZ(),
		this->getX()*other.getY() - this->getY()*other.getX()
	);
	return res;
}

float CVector3::dot(const CVector3 & other)
{
	return (getX() * other.getX() + getY() * other.getY() + getZ() * other.getZ());
}

CVector3 CVector3::operator-(const CVector3 & other)
{
	CVector3 res;
	res.setValues(
		this->getX() - other.getX(),
		this->getY() - other.getY(),
		this->getZ() - other.getZ()
	);
	return res;
}

bool CVector3::operator!=(const CVector3 & other)
{
	return m_X != other.m_X || m_Y != other.m_Y || m_Z != other.m_Z;
}

bool CVector3::operator==(const CVector3 & other) const
{
	return m_X == other.m_X && m_Y == other.m_Y && m_Z == other.m_Z;
}

/*
*/
CVector3::~CVector3()
{
}

/*
*/
void CVector3::setValues(float _x, float _y, float _z)
{
	m_X = _x;
	m_Y = _y;
	m_Z = _z;
}

/*
*/
void CVector3::setValues(float *values)
{
	m_X = *(values + 0);
	m_Y = *(values + 1);
	m_Z = *(values + 2);
}

void CVector3::rotate(float dx, float dy)
{
	float cosX = cosf(dx);
	float sinX = sinf(dx);
	float cosY = cosf(-dy);
	float sinY = sinf(-dy);

	float X = m_X * cosX - m_Z * sinX;
	float Y = m_X * sinX * sinY + m_Y * cosY + m_Z * cosX * sinY;
	float Z = m_X * sinX * cosY - m_Y * sinY + m_Z * cosY * cosX;

	setValues(X, Y, Z);
}

