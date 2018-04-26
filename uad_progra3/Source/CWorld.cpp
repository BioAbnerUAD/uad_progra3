#include "../Include/CWorld.h"
#include "../Include/CLogger.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/Globals.h"
#include <fstream>

CWorld::CWorld(COpenGLRenderer* renderer)
{
	isInitialized = false;
	idObject.push_back(new CWorldIdObject(0));
	idObject.push_back(new CWorldIdObject(1));
	this->renderer = renderer;
}

CWorld::~CWorld()
{
	// Free up graphics memory
	for (size_t i = 0; i < idObject.size(); i++) delete idObject[i];
	if (isInitialized)
	{
		delete cubeGrid;
	}
}

bool CWorld::initialize()
{
	cubeGrid = new CCubeGrid();

#pragma region Generate_World
	/*---This area should be modified later to randomly generate World---*/
	for (int i = -GRID_SIZE / 2; i < GRID_SIZE / 2; i++)
		for (int j = -GRID_SIZE / 2; j < GRID_SIZE / 2; j++)
		{
			cubeGrid->addCell(new CCell(CVector3((float)i, 0, (float)j), idObject[0]));
			cubeGrid->addCell(new CCell(CVector3((float)i, 1, (float)j), idObject[0]));
		}

#pragma endregion Generate_World

	isInitialized = true;
	return isInitialized;
}

void CWorld::render(CVector3 camPosition, CVector3 camRotation)
{
	vector<void*> params;
	params.push_back(this);
	params.push_back(&camPosition);
	params.push_back(&camRotation);

	cubeGrid->getCells()->ForEach([](vector<void*> params, void* item) {
		auto cell = (CCell*)item;
		auto lthis = (CWorld*)params[0];
		auto camPosition = (CVector3*)params[1];
		auto camRotation = (CVector3*)params[2];

		// Get a matrix that has both the object rotation and translation
		MathHelper::Matrix4 modelMatrix
			= MathHelper::FirstPersonModelMatrix((float)camRotation->getX(), (float)camRotation->getY(),
			(*camPosition) + cell->centro);

		lthis->renderer->renderMCCube(&modelMatrix);
	}, params);
}

bool CWorld::save(string filename)
{
	if (isInitialized)
	{
		ofstream stream(filename, ios::binary);

		auto header = FILE_HEADER;
		byte version = VERSION_NO;
		byte subversion = SUB_VERSION_NO;

		stream.write(header, strlen(header));
		stream.write((char*)&version, sizeof(byte));
		stream.write((char*)&subversion, sizeof(byte));

		size_t chunkSize = cubeGrid->cellCount();

		stream.write((char*)&chunkSize, sizeof(size_t));

		cubeGrid->getCells()->ForEach([](void* obj, void* item) {
			auto stream = (ofstream*)obj;
			auto cell = (CCell*)item;
			float x = cell->centro.getX(), y = cell->centro.getY(), z = cell->centro.getZ();
			stream->write((char*)&x, sizeof(float));
			stream->write((char*)&y, sizeof(float));
			stream->write((char*)&z, sizeof(float));

			int id = cell->instance->worldObjectID;
			stream->write((char*)&id, sizeof(int));
		}, (void*)&stream);

		stream.close();

		return true;
	}
}

bool CWorld::load(string filename)
{
	if (!isInitialized)
	{
		cubeGrid = new CCubeGrid();
		CWorldIdObject* idObj = new CWorldIdObject(0);
		idObject.push_back(idObj);

		ifstream stream(filename.c_str(), ios::binary);

		char* header = new char[strlen(FILE_HEADER)];
		byte version;
		byte subversion;

		stream.read(header, strlen(FILE_HEADER));
		header[strlen(FILE_HEADER)] = '\0';
		if (strcmp(header, FILE_HEADER) != 0)
		{
			Log << "Error: Wrong World File Header" << endl;
			return false;
		}
		stream.read((char*)&version, sizeof(byte));
		stream.read((char*)&subversion, sizeof(byte));
		if (version > VERSION_NO || (version == VERSION_NO && subversion > SUB_VERSION_NO))
		{
			Log << "Error: Unsupported World File Version" << endl;
			return false;
		}

		size_t chunkSize;

		stream.read((char*)&chunkSize, sizeof(size_t));

		for (size_t i = 0; i < chunkSize; i++)
		{
			float chunkX, chunkY, chunkZ;
			stream.read((char*)&chunkX, sizeof(float));
			stream.read((char*)&chunkY, sizeof(float));
			stream.read((char*)&chunkZ, sizeof(float));

			auto chunk = new CCell(CVector3(
				(float)chunkX, (float)chunkY, (float)chunkZ), idObject[0]);

			int id;
			stream.read((char*)&id, sizeof(int));
			chunk->instance->worldObjectID = id;

			cubeGrid->addCell(chunk);
		}

		isInitialized = true;

		stream.close();
		return true;
	}
	return false;
}
