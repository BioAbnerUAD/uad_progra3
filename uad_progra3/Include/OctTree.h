#pragma once
#include "CVector3.h"
#include <stdexcept>


struct Zone
{
	CVector3 center;
	CVector3 halfSize;

	Zone(CVector3 center, CVector3 halfSize) : center(center), halfSize(halfSize) {};
	bool contains(CVector3 point)
	{
		return (
			point.getX() < center.getX() + halfSize.getX() && 
			point.getX() > center.getX() - halfSize.getX() && 
			point.getY() < center.getY() + halfSize.getY() && 
			point.getY() > center.getY() - halfSize.getY() &&
			point.getZ() < center.getZ() + halfSize.getZ() && 
			point.getZ() > center.getZ() - halfSize.getZ() );
	}
};

template<typename T>
struct Data
{
	CVector3 pos;
	T* load;

	Data(CVector3 pos, T* data) : pos(pos), load(data) {};
};

template<typename T>
class OctTree
{
public:
	//Top North East
	OctTree* tne;
	//Top North West
	OctTree* tnw;
	//Top South East
	OctTree* tse;
	//Top South West
	OctTree* tsw;
	//Bottom North East
	OctTree* bne;
	//Bottom North West
	OctTree* bnw;
	//Bottom South East
	OctTree* bse;
	//Bottom South West
	OctTree* bsw;

	//Apuntadores a las celdas
	Data<T> objects[8];

	OctTree(Zone boundary) : boundary(boundary) {};
	~OctTree();
	void Insert(T* data, CVector3 pos);

private:
	Zone boundary;
	unsigned short count = 0;
	bool divided = false;
	static const unsigned short CAPACITY = 8;

	void Insert(Data<T> item);
	void Subdivide();
};

template<typename T>
inline OctTree<T>::~OctTree()
{
	delete tne;
	delete tnw;
	delete tse;
	delete tsw;
	delete bne;
	delete bnw;
	delete bse;
	delete bsw;
}

template<typename T>
inline void OctTree<T>::Insert(T * data, CVector3 pos)
{
	Insert(Data<T>(pos, data));
}

template<typename T>
inline void OctTree<T>::Insert(Data<T> item)
{
	if (this->count < CAPACITY)
	{
		objects[count] = item;
		count++;
	}
	else
	{
		if (!this->divided)
		{
			this->Subdivide();

			if (tne->boundary.contains(item.pos)) tne->Insert(item);
			else if (tnw->boundary.contains(item.pos)) tnw->Insert(item);
			else if (tse->boundary.contains(item.pos)) tse->Insert(item);
			else if (tsw->boundary.contains(item.pos)) tsw->Insert(item);
			else if (bne->boundary.contains(item.pos)) bne->Insert(item);
			else if (bnw->boundary.contains(item.pos)) bnw->Insert(item);
			else if (bse->boundary.contains(item.pos)) bse->Insert(item);
			else if (bsw->boundary.contains(item.pos)) bsw->Insert(item);

			else throw new std::invalid_argument("Item does not belong in this boundary.");
		}

	}
}

template<typename T>
inline void OctTree<T>::Subdivide()
{
	CVector3 qCenter;
	CVector3 qSize = CVector3(boundary.halfSize.getX() / 2, boundary.halfSize.getY() / 2, boundary.halfSize.getZ() / 2);

	qCenter = CVector3(boundary.center.getX() + qSize.getX(), boundary.center.getY() + qSize.getY(), boundary.center.getZ() + qSize.getZ());
	tne = new OctTree(Zone(qCenter, qSize));

	qCenter = CVector3(boundary.center.getX() - qSize.getX(), boundary.center.getY() + qSize.getY(), boundary.center.getZ() + qSize.getZ());
	tnw = new OctTree(Zone(qCenter, qSize));

	qCenter = CVector3(boundary.center.getX() + qSize.getX(), boundary.center.getY() + qSize.getY(), boundary.center.getZ() - qSize.getZ());
	tse = new OctTree(Zone(qCenter, qSize));

	qCenter = CVector3(boundary.center.getX() - qSize.getX(), boundary.center.getY() + qSize.getY(), boundary.center.getZ() - qSize.getZ());
	tsw = new OctTree(Zone(qCenter, qSize));

	qCenter = CVector3(boundary.center.getX() + qSize.getX(), boundary.center.getY() - qSize.getY(), boundary.center.getZ() + qSize.getZ());
	bne = new OctTree(Zone(qCenter, qSize));

	qCenter = CVector3(boundary.center.getX() - qSize.getX(), boundary.center.getY() - qSize.getY(), boundary.center.getZ() + qSize.getZ());
	bnw = new OctTree(Zone(qCenter, qSize));

	qCenter = CVector3(boundary.center.getX() + qSize.getX(), boundary.center.getY() - qSize.getY(), boundary.center.getZ() - qSize.getZ());
	bse = new OctTree(Zone(qCenter, qSize));

	qCenter = CVector3(boundary.center.getX() - qSize.getX(), boundary.center.getY() - qSize.getY(), boundary.center.getZ() - qSize.getZ());
	bsw = new OctTree(Zone(qCenter, qSize));

	divided = true;
}