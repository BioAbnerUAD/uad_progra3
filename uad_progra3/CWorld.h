#pragma once
#include "CWorldIdObject.h"
#include "CCubeGrid.h"
#include "CChunk.h"
#include <vector>

#define FILE_HEADER "MAINCRA UORL FA-IL" //:V
#define VERSION_NO 1
#define SUB_VERSION_NO 1


class CWorld
{
public:
	CWorld();
	~CWorld();
	bool initialize();
	void render();
	void save();
	void load();
	vector<CWorldIdObject*> idObject;
	CCubeGrid* cubeGrid;
	bool isInitialized;
};

