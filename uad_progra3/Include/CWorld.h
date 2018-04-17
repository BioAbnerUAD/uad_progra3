#pragma once
#include "CWorldIdObject.h"
#include "CCubeGrid.h"
#include "CChunk.h"
#include "COpenGLRenderer.h"
#include <vector>

#define FILE_HEADER "MAINCRA UORL FA-IL" //:V
#define VERSION_NO 1
#define SUB_VERSION_NO 1


class CWorld
{
public:
	CWorld();
	~CWorld();
	bool initialize(COpenGLRenderer* renderer);
	void render(CVector3 camPosition);
	void save();
	void load();
	vector<CWorldIdObject*> idObject;
	CCubeGrid* cubeGrid;
	bool isInitialized;
private:
	COpenGLRenderer* renderer;

	unsigned int shaderProgramID = 0;
	unsigned int VAOID = 0;

	float* getVertices();
	size_t getNumVertices();
	unsigned short* getVertexIndices();
	size_t getNumFaces();
};

