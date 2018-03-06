#include "CWorld.h"
#include "Include\CLogger.h"
#include <fstream>

CWorld::CWorld()
{
	
}


CWorld::~CWorld()
{
	for (size_t i = 0; i < idObject.size(); i++)
	{
		delete idObject[i];
	}
}

const CWorldIdObject * CWorld::getIdObject(int ID) const
{
	return idObject[ID];
}

void CWorld::addIdObject(CWorldIdObject * object)
{
	idObject.push_back(object);
}

CChunk * CWorld::getChunk(int x, int y)
{
	string key = to_string(x) + "_" + to_string(y);
	if (grid.find(key) == grid.end()) {
		grid.insert(pair<string, CChunk>(key, CChunk()));
	}
	return &grid[key];
}

void CWorld::save()
{
	ofstream stream("world.bin", ios::binary);

	vector<string> chunkKeys;
	vector<CChunk> vecChunks;

	auto header = FILE_HEADER;
	int version = VERSION_NO;

	stream.write(&header[0], strlen(header));
	stream.write((char*)&version, sizeof(int));

	for (auto chunk: grid) {
		chunkKeys.push_back(chunk.first);
		vecChunks.push_back(chunk.second);
	}

	int chunkSize = chunkKeys.size();

	stream.write((char*)&chunkSize, sizeof(int));

	for (auto key: chunkKeys)
	{
		int length = key.length();
		stream.write((char*)&length, sizeof(int));
		stream.write(&key[0], key.length());
	}
	stream.write((char*)&vecChunks[0], chunkSize * sizeof(CChunk));

	stream.close();
}

void CWorld::load()
{
}
