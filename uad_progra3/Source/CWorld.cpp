#include "../Include/CWorld.h"
#include "../Include/CLogger.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/Globals.h"
#include "../Include/LoadTGA.h"
#include <fstream>

CWorld::CWorld()
{
	isInitialized = false;
}

CWorld::~CWorld()
{
	// Free up graphics memory

	cubeGrid->getChunks()->ForEach([](void* obj, void* item) {
		auto lthis = (CWorld*)obj;
		auto chunk = (CChunk*)item;

		lthis->renderer->freeGraphicsMemoryForObject(&chunk->shaderProgramID, &chunk->VAOID);
	}, (void*)this);

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
	idObject.push_back(new CWorldIdObject(0));
	idObject.push_back(new CWorldIdObject(1));

	for (int i = -2; i < 3; i++)
		for (int j = -2; j < 1; j++)
			for (int k = -2; k < 3; k++)
			{
				cubeGrid->addChunk(new CChunk(idObject[j == 0 ? 0 : 1], i, j, k));
			}
#pragma endregion Generate_World

	this->renderer = renderer;

	std::wstring wresourceFilenameTexture;
	std::string resourceFilenameTexture;

	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;

	std::string resourceFilenameVS;
	std::string resourceFilenameFS;

	// If resource files cannot be found, return
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_MC_CUBE, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_MC_CUBE, wresourceFilenameFS, resourceFilenameFS))
	{
		Log << "ERROR: Unable to find one or more resources: " << endl;
		Log << "  " << VERTEX_SHADER_MC_CUBE << endl;
		Log << "  " << FRAGMENT_SHADER_MC_CUBE << endl;

		return false;
	}
	if (!CWideStringHelper::GetResourceFullPath(MC_CUBE_TEXTURE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << MC_CUBE_TEXTURE << endl;
		return false;
	}

	// Initialize the texture
	unsigned int mcCUbeTextureID = -1;

	TGAFILE tgaFile;
	tgaFile.imageData = NULL;

	if (LoadTGAFile(resourceFilenameTexture.c_str(), &tgaFile))
	{
		if (tgaFile.imageData == NULL ||
			tgaFile.imageHeight < 0 ||
			tgaFile.imageWidth < 0)
		{
			if (tgaFile.imageData != NULL)
			{
				delete[] tgaFile.imageData;
			}
			return false;
		}

		// Create a texture object for the menu, and copy the texture data to graphics memory
		if (!renderer->createTextureObject(
			&mcCUbeTextureID,
			tgaFile.imageData,
			tgaFile.imageWidth,
			tgaFile.imageHeight
		))
		{
			return false;
		}

		// Texture data is stored in graphics memory now, we don't need this copy anymore
		if (tgaFile.imageData != NULL)
		{
			delete[] tgaFile.imageData;
		}
	}
	else
	{
		// Free texture data
		if (tgaFile.imageData != NULL)
		{
			delete[] tgaFile.imageData;
		}

		return false;
	}

	vector<void*> params;
	params.push_back(this);
	params.push_back(&resourceFilenameVS);
	params.push_back(&resourceFilenameFS);
	params.push_back(&mcCUbeTextureID);

	cubeGrid->getChunks()->ForEach([](vector<void*> params, void* item) {
		auto chunk = (CChunk*)item;
		auto lthis = (CWorld*)params[0];
		auto resourceFilenameVS = (std::string*) params[1];
		auto resourceFilenameFS = (std::string*) params[2];
		chunk->textureID = *((unsigned int*) params[3]);

		lthis->renderer->initializeMCObjects(
			&chunk->shaderProgramID, &chunk->VAOID,
			resourceFilenameVS->c_str(), resourceFilenameFS->c_str(),
			chunk->getVertices(),
			(int)chunk->getNumVertices(),
			chunk->getUVs(),
			chunk->getVertexIndices(),
			(int)chunk->getNumFaces()
		);
	}, params);

	return isInitialized;
}

void CWorld::render(CVector3 camPosition, CVector3 camRotation)
{
	// Get a matrix that has both the object rotation and translation
	MathHelper::Matrix4 modelMatrix 
		= MathHelper::FirstPersonModelMatrix((float)camRotation.getX(), (float)camRotation.getY(), camPosition);

	vector<void*> params;
	params.push_back(this);
	params.push_back(&modelMatrix);

	cubeGrid->getChunks()->ForEach([](vector<void*> params, void* item) {
		auto chunk = (CChunk*)item;
		auto lthis = (CWorld*)params[0];
		auto modelMatrix = (MathHelper::Matrix4*) params[1];

		lthis->renderer->renderMCObjects(
			&chunk->textureID, &chunk->shaderProgramID, &chunk->VAOID, (int)chunk->getNumFaces(), modelMatrix);
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
