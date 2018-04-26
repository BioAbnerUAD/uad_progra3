#pragma once
#include "OctTree.h"
#include "CCell.h"

#define GRID_SIZE 16
#define GRID_HEIGHT 2

class CCubeGrid
{
public:
	CCubeGrid();
	~CCubeGrid();
	CCell * getCell(CVector3 pos);
	CCell * getCell(float x, float y, float z);
	bool deleteCell(CVector3 pos);
	bool deleteCell(float x, float y, float z);
	void addCell(CCell* chunk);
	size_t cellCount();
	OctTree<CCell>* getCells();
private:
	OctTree<CCell> cells;
};

