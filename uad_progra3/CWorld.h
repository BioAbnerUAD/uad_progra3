#pragma once
#include "CWorldIdObject.h"
#include "CWorldObjectInstance.h"
#include "CChunk.h"
#include <vector>
#include <map>

#define FILE_HEADER "MINECRAFT WORLD FILE"
#define VERSION_NO 1


class CWorld
{
public:
	CWorld();
	~CWorld();
	const CWorldIdObject * getIdObject(int ID) const;
	void addIdObject(CWorldIdObject * object);
	CChunk * getChunk(int x, int y);
	void save();
	void load();
private:
	vector<CWorldIdObject*> idObject;
	map<std::string, CChunk> grid;
};

