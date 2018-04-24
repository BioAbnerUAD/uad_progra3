#include "../Include/CWorld.h"
#include "../Include/CLogger.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/Globals.h"
#include <fstream>

CWorld::CWorld()
{
	isInitialized = false;
	idObject.push_back(new CWorldIdObject(0));
	idObject.push_back(new CWorldIdObject(1));
}

CWorld::~CWorld()
{
	// Free up graphics memory
	if (isInitialized)
	{
		for (size_t i = 0; i < idObject.size(); i++) delete idObject[i];
		delete cubeGrid;
	}
}

bool CWorld::initialize(COpenGLRenderer* renderer)
{
	cubeGrid = new CCubeGrid();

#pragma region Generate_World
	/*---This area should be modified later to randomly generate World---*/
	cubeGrid->addChunk(new CChunk(idObject[0], 0, 0, 0));
#pragma endregion Generate_World

	this->renderer = renderer;

	isInitialized = true;
	return isInitialized;
}

void CWorld::render(CVector3 camPosition, CVector3 camRotation)
{
	vector<void*> params;
	params.push_back(this);
	params.push_back(&camPosition);
	params.push_back(&camRotation);

	cubeGrid->getChunks()->ForEach([](vector<void*> params, void* item) {
		auto chunk = (CChunk*)item;
		auto lthis = (CWorld*)params[0];
		auto camPosition = (CVector3*)params[1];
		auto camRotation = (CVector3*)params[2];

		for (int i = 0; i < CHUNK_SIZE; i++)
			for (int j = 0; j < CHUNK_HEIGHT; j++)
				for (int k = 0; k < CHUNK_SIZE; k++)
				{
					// Get a matrix that has both the object rotation and translation
					MathHelper::Matrix4 modelMatrix
						= MathHelper::FirstPersonModelMatrix((float)camRotation->getX(), (float)camRotation->getY(),
							(*camPosition) + chunk->blocks[i][j][k].centro);

					lthis->renderer->renderMCCube(&modelMatrix);
				}
	}, params);
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

		cubeGrid->getChunks()->ForEach([](void* obj, void* item) {
			auto stream = (ofstream*)obj;
			auto chunk = (CChunk*)item;
			stream->write((char*)&(chunk->x), sizeof(int));
			stream->write((char*)&(chunk->y), sizeof(int));
			stream->write((char*)&(chunk->z), sizeof(int));

			for (size_t i = 0; i < CHUNK_SIZE; i++)
			{
				for (size_t j = 0; j < CHUNK_HEIGHT; j++)
				{
					for (size_t k = 0; k < CHUNK_SIZE; k++)
					{
						int id = chunk->blocks[i][j][k].instance->worldObjectID;
						stream->write((char*)&id, sizeof(int));
					}
				}
			}
		}, (void*)&stream);

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
			int chunkX, chunkY, chunkZ;
			stream.read((char*)&chunkX, sizeof(int));
			stream.read((char*)&chunkY, sizeof(int));
			stream.read((char*)&chunkZ, sizeof(int));

			auto chunk = new CChunk(idObject[0], chunkX, chunkY, chunkZ);

			for (size_t i = 0; i < CHUNK_SIZE; i++)
			{
				for (size_t j = 0; j < CHUNK_HEIGHT; j++)
				{
					for (size_t k = 0; k < CHUNK_SIZE; k++)
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
