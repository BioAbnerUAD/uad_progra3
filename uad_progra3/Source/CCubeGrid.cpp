#include "../Include/CCubeGrid.h"
#include <string>

CCubeGrid::CCubeGrid() : cells(Zone(CVector3(0, 0, 0),
	CVector3(GRID_SIZE, GRID_HEIGHT, GRID_SIZE)))
{
}


CCubeGrid::~CCubeGrid()
{
}

CCell * CCubeGrid::getCell(CVector3 pos)
{
	return cells.GetIndex(pos);
}

CCell * CCubeGrid::getCell(float x, float y, float z)
{
	CVector3 pos(x, y, z);
	return cells.GetIndex(pos);
}

bool CCubeGrid::deleteCell(CVector3 pos)
{
	return cells.DeleteIndex(pos);
}

bool CCubeGrid::deleteCell(float x, float y, float z)
{
	return cells.DeleteIndex(CVector3(x, y, z));
}

void CCubeGrid::addCell(CCell * cell)
{
	cells.Insert(cell, CVector3(
		(float)cell->centro.getX(), (float)cell->centro.getY(), (float)cell->centro.getZ()));
}

size_t CCubeGrid::cellCount()
{
	return cells.size();
}

OctTree<CCell>* CCubeGrid::getCells()
{
	return &cells;
}
