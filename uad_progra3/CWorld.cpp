#include "CWorld.h"
#include "Include\CLogger.h"
#include "Include\CWideStringHelper.h"
#include "Include\Globals.h"
#include <fstream>

CWorld::CWorld()
{
	isInitialized = false;
}

CWorld::~CWorld()
{
	// Free up graphics memory

	renderer->freeGraphicsMemoryForObject( &shaderProgramID, &VAOID);

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

	this->renderer = renderer;

	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;
	std::string resourceFilenameVS;
	std::string resourceFilenameFS;

	// If resource files cannot be found, return
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_WIREFRAME, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_WIREFRAME, wresourceFilenameFS, resourceFilenameFS))
	{
		Log << "ERROR: Unable to find one or more resources: " << endl;
		Log << "  " << VERTEX_SHADER_WIREFRAME << endl;
		Log << "  " << FRAGMENT_SHADER_WIREFRAME << endl;

		return false;
	}

	renderer->createShaderProgram(&shaderProgramID, resourceFilenameVS.c_str(), resourceFilenameFS.c_str());

	isInitialized = renderer->allocateGraphicsMemoryForObject(
		&shaderProgramID, &VAOID,
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

	// Get a matrix that has both the object rotation and translation
	MathHelper::Matrix4 modelMatrix = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, camPosition);

	renderer->renderWireframeObject(&shaderProgramID, &VAOID, getNumFaces(), color, &modelMatrix);
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
	return cubeGrid->getChunk(0,0)->m_verticesRaw;
}

size_t CWorld::getNumVertices()
{
	return cubeGrid->getChunk(0, 0)->m_numVertices;
}

unsigned short * CWorld::getVertexIndices()
{
	return cubeGrid->getChunk(0, 0)->m_vertexIndices;
}

size_t CWorld::getNumFaces()
{
	return cubeGrid->getChunk(0, 0)->m_numFaces;
}
