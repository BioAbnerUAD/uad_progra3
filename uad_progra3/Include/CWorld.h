#pragma once
#include "CWorldIdObject.h"
#include "CCubeGrid.h"
#include "COpenGLRenderer.h"
#include <vector>

#define FILE_HEADER "MAINCRA UORL FA-IL" //:V
#define VERSION_NO 3
#define SUB_VERSION_NO 2


class CWorld
{
public:
	CWorld::CWorld(COpenGLRenderer* renderer);
	~CWorld();
	bool initialize();
	void render(CVector3 camPosition, CVector3 camRotation);
	bool save(string filename);
	bool load(string filename);
	vector<CWorldIdObject*> idObject;
	CCubeGrid* cubeGrid;
	bool isInitialized;
private:
	COpenGLRenderer* renderer;
};

