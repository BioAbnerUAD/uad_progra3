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
			point.getX() <= center.getX() + halfSize.getX() && 
			point.getX() > center.getX() - halfSize.getX() && 
			point.getY() <= center.getY() + halfSize.getY() && 
			point.getY() > center.getY() - halfSize.getY() &&
			point.getZ() <= center.getZ() + halfSize.getZ() && 
			point.getZ() > center.getZ() - halfSize.getZ() );
	}
};

typedef void(*OctTreeForEach)(void* item);
typedef void(*OctTreeForEachO)(void* obj, void* item);
typedef void(*OctTreeForEachP)(std::vector<void*> params, void* item);

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

	//Informacion de la celda
	T * data = nullptr;
	CVector3 pos;

	OctTree(Zone boundary) : boundary(boundary) {};
	~OctTree();
	void Insert(T* data, CVector3 pos);
	size_t size();
	Zone getBoundary();

	void ForEach(OctTreeForEach callback);
	void ForEach(OctTreeForEachO callback, void* callbackObj);
	void ForEach(OctTreeForEachP callback, std::vector<void*> params);
	T* GetIndex(CVector3 pos);

private:
	Zone boundary;
	bool divided = false;
	bool hasElement = false;
	size_t _totalSize = 0;
	void Subdivide();
};

template<typename T>
inline OctTree<T>::~OctTree()
{
	if (divided)
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
	else
	{
		if(hasElement) delete data;
	}
}

template<typename T>
inline void OctTree<T>::Insert(T * data, CVector3 pos)
{
	if (!hasElement)
	{
		_totalSize++;
		this->data = data;
		this->pos = pos;
		hasElement = true;
	}
	else
	{
		if (!this->divided)
		{
			this->Subdivide();

			_totalSize--;

			this->Insert(this->data, this->pos);
			this->Insert(data, pos);
		}
		else
		{
			_totalSize++;

			if (tne->boundary.contains(pos)) tne->Insert(data, pos);
			else if (tnw->boundary.contains(pos)) tnw->Insert(data, pos);
			else if (tse->boundary.contains(pos)) tse->Insert(data, pos);
			else if (tsw->boundary.contains(pos)) tsw->Insert(data, pos);
			else if (bne->boundary.contains(pos)) bne->Insert(data, pos);
			else if (bnw->boundary.contains(pos)) bnw->Insert(data, pos);
			else if (bse->boundary.contains(pos)) bse->Insert(data, pos);
			else if (bsw->boundary.contains(pos)) bsw->Insert(data, pos);

			else throw new std::invalid_argument("Item does not belong in this boundary.");
		}

	}
}

template<typename T>
inline size_t OctTree<T>::size()
{
	return _totalSize;
}

template<typename T>
inline Zone OctTree<T>::getBoundary()
{
	return boundary;
}

template<typename T>
inline void OctTree<T>::ForEach(OctTreeForEach callback)
{
	if (!this->divided)
	{
		if (this->hasElement) callback((void*)this->data);
	}
	else
	{
		tne->ForEach(callback);
		tnw->ForEach(callback);
		tse->ForEach(callback);
		tsw->ForEach(callback);
		bne->ForEach(callback);
		bnw->ForEach(callback);
		bse->ForEach(callback);
		bsw->ForEach(callback);
	}
}

template<typename T>
inline void OctTree<T>::ForEach(OctTreeForEachO callback, void * callbackObj)
{
	if (!this->divided)
	{
		if (this->hasElement) callback(callbackObj, (void*)this->data);
	}
	else
	{
		tne->ForEach(callback, callbackObj);
		tnw->ForEach(callback, callbackObj);
		tse->ForEach(callback, callbackObj);
		tsw->ForEach(callback, callbackObj);
		bne->ForEach(callback, callbackObj);
		bnw->ForEach(callback, callbackObj);
		bse->ForEach(callback, callbackObj);
		bsw->ForEach(callback, callbackObj);
	}
}

template<typename T>
inline void OctTree<T>::ForEach(OctTreeForEachP callback, std::vector<void*> params)
{
	if (!this->divided)
	{
		if (this->hasElement) callback(params, (void*)this->data);
	}
	else
	{
		tne->ForEach(callback, params);
		tnw->ForEach(callback, params);
		tse->ForEach(callback, params);
		tsw->ForEach(callback, params);
		bne->ForEach(callback, params);
		bnw->ForEach(callback, params);
		bse->ForEach(callback, params);
		bsw->ForEach(callback, params);
	}
}

template<typename T>
inline T * OctTree<T>::GetIndex(CVector3 pos)
{
	if (!this->divided) return data;
	else if (this->tne->boundary.contains(pos)) return this->tne->GetIndex(pos);
	else if (this->tnw->boundary.contains(pos)) return this->tnw->GetIndex(pos);
	else if (this->tse->boundary.contains(pos)) return this->tse->GetIndex(pos);
	else if (this->tsw->boundary.contains(pos)) return this->tsw->GetIndex(pos);
	else if (this->bne->boundary.contains(pos)) return this->bne->GetIndex(pos);
	else if (this->bnw->boundary.contains(pos)) return this->bnw->GetIndex(pos);
	else if (this->bse->boundary.contains(pos)) return this->bse->GetIndex(pos);
	else if (this->bsw->boundary.contains(pos)) return this->bsw->GetIndex(pos);
	else throw new std::invalid_argument("Item does not belong in this boundary.");
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