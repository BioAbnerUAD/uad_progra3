#include "CWorld.h"
#include "Include\CLogger.h"
#include <fstream>

CWorld::CWorld()
{
	isInitialized = false;
}

CWorld::~CWorld()
{
	// Free up graphics memory

	size_t ID = 0;
	renderer->freeGraphicsMemoryForObject( &ID, &ID);

	if (isInitialized)
	{
		for (size_t i = 0; i < idObject.size(); i++) delete idObject[i];
		delete cubeGrid;
	}
}

bool CWorld::initialize(COpenGLRenderer* renderer)
{
	cubeGrid = new CCubeGrid();
	CWorldIdObject* idObj = new CWorldIdObject(0);
	idObject.push_back(idObj);

	CChunk* chunk = new CChunk(&idObject, 0, 0);

	cubeGrid->addChunk(chunk);

	size_t ID = 0;

	isInitialized = renderer->allocateGraphicsMemoryForObject(
		&ID, &ID, 
		getVertices(),
		getNumVertices(),
		getVertexIndices(),
		getNumFaces()
	);

	return isInitialized;
}

void CWorld::render(CVector3 camPosition)
{
	// White 
	float color[3] = { 0.95f, 0.95f, 0.95f };

	// convert total degrees rotated to radians;
	//not using this yet
	double totalDegreesRotatedRadians = 0 * 3.1459 / 180.0;

	CVector3 zero = {0,0,0};
	// Get a matrix that has both the object rotation and translation
	MathHelper::Matrix4 modelMatrix = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, zero);

	size_t ID = 0;
	renderer->renderObject(&ID, &ID, getNumFaces(), color, &modelMatrix);
}

void CWorld::save()
{
	if (isInitialized)
	{
		ofstream stream("world.bin", ios::binary);

		auto header = FILE_HEADER;
		size_t version = VERSION_NO;
		size_t subversion = SUB_VERSION_NO;

		stream.write(header, strlen(header));
		stream.write((char*)&version, sizeof(size_t));
		stream.write((char*)&subversion, sizeof(size_t));

		size_t chunkSize = cubeGrid->chunksSize();

		stream.write((char*)&chunkSize, sizeof(size_t));
		auto chunks = *(cubeGrid->getChunks());

		for (auto chunk : chunks)
		{
			stream.write((char*)&chunk.second->x, sizeof(int));
			stream.write((char*)&chunk.second->y, sizeof(int));

			for (size_t i = 0; i < CHUNK_SIZE; i++)
			{
				for (size_t j = 0; j < CHUNK_SIZE; j++)
				{
					for (size_t k = 0; k < CHUNK_HEIGHT; k++)
					{
						int id = chunk.second->blocks[i][j][k].instance->worldObjectID;
						stream.write((char*)&id, sizeof(int));
					}
				}
			}
		}

		stream.close();
	}
}

void CWorld::load()
{
	if (!isInitialized)
	{
		cubeGrid = new CCubeGrid();
		CWorldIdObject* idObj = new CWorldIdObject(0);
		idObject.push_back(idObj);

		ifstream stream("world.bin", ios::binary);

		char* header = new char[strlen(FILE_HEADER)];
		size_t version;
		size_t subversion;

		stream.read(header, strlen(FILE_HEADER));
		if (strcmp(header, FILE_HEADER) != 0)
		{
			Log << "Error: Wrong World File Header" << endl;
			return;
		}
		stream.read((char*)&version, sizeof(size_t));
		stream.read((char*)&subversion, sizeof(size_t));
		if (version > VERSION_NO || (version == VERSION_NO && subversion > SUB_VERSION_NO))
		{
			Log << "Error: Unsupported World File Version" << endl;
			return;
		}

		size_t chunkSize;

		stream.read((char*)&chunkSize, sizeof(size_t));

		for (size_t i = 0; i < chunkSize; i++)
		{
			int chunkX, chunkY;
			stream.read((char*)&chunkX, sizeof(int));
			stream.read((char*)&chunkY, sizeof(int));

			CChunk* chunk = new CChunk(&idObject, chunkX, chunkY);

			for (size_t i = 0; i < CHUNK_SIZE; i++)
			{
				for (size_t j = 0; j < CHUNK_SIZE; j++)
				{
					for (size_t k = 0; k < CHUNK_HEIGHT; k++)
					{
						int id;
						stream.read((char*)&id, sizeof(int));
						chunk->blocks[i][j][k].instance->worldObjectID = id;
					}
				}
			}

			cubeGrid->addChunk(chunk);
		}

		isInitialized = true;

		stream.close();
	}
}

float * CWorld::getVertices()
{
	return &(cubeGrid->getChunk(0,0)->trigsRaw)[0];
}

size_t CWorld::getNumVertices()
{
	return cubeGrid->getChunk(0, 0)->trigsRaw.size();
}

unsigned short * CWorld::getVertexIndices()
{
	return &(cubeGrid->getChunk(0, 0)->indicesTrigs)[0];
}

size_t CWorld::getNumFaces()
{
	return cubeGrid->getChunk(0, 0)->indicesTrigs.size() / 3;
}
