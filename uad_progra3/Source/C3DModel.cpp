#include "../stdafx.h"

#include <algorithm>

#include "../Include/CLogger.h"
#include "C3DModel_Obj.h"
#include "../Include/C3DModel_3DS.h"
#include "../Include/C3DModel.h"
#include "../Include/UTFConvert.h"

/* */
C3DModel::C3DModel()
	: m_vertexIndices(NULL),
	m_normalIndices(NULL),
	m_UVindices(NULL),
	m_vertices(NULL),
	m_verticesRaw(NULL),
	m_normals(NULL),
	m_normalsRaw(NULL),
	m_UVCoords(NULL),
	m_uvCoordsRaw(NULL),
	m_Initialized(false),
	m_numVertices(0), 
	m_numNormals(0), 
	m_numUVCoords(0), 
	m_numFaces(0),
	m_currentVertex(0),
	m_currentNormal(0),
	m_currentUV(0),
	m_currentFace(0),
	m_graphicsMemoryObjectId(0),
	m_shaderProgramId(0)
{
	Log << "Constructor: C3DModel()" << endl;
}

/* */
C3DModel::~C3DModel()
{
	Log << "Destructor: C3DModel()" << endl;
}

bool C3DModel::loadFromFile(const wchar_t * const filename)
{
	bool readFileOk = false;

	// Free any previous resources
	reset();

	// First pass is to count the number of vertices, normals, UVs, faces
	readFileOk = readFile(UTFConvert::convert_UTF16_UTF8((wchar_t*)filename));

	if (readFileOk)
	{
		// Display count
		Log << "Finished reading 3D model" << endl;
		Log << "Vertices: " << m_numVertices << endl;
		Log << "Normals: " << m_numNormals << endl;
		Log << "UVCoords: " << m_numUVCoords << endl;
		Log << "Faces: " << m_numFaces << endl;
		Log << this->toString() << endl;

		// Check for MAX number of faces
		if (m_numVertices >= 65535 || m_numNormals >= 65535 || m_numUVCoords >= 65535)
		{
			Log << "Error: object cannot have more than 65535 vertices" << endl;
			Log << "Object attempted to load has: " << m_numVertices << " vertices" << endl;
			return false;
		}

		// If model was read without normals or UVCoords, we'll set a default value for them
		// i.e.:
		//   0,0 for UV coords
		//   face normal for normal
		if (m_numNormals == 0)
		{
			m_numNormals = m_numFaces;
			computeFaceNormals();
		}
		if (m_numUVCoords == 0)
		{
			m_numUVCoords = m_numVertices;
		}

		m_Initialized = true;
	}
	else
	{
		Log << "Error ocurred while reading 3D model." << endl;
	}

	return readFileOk;
}

C3DModel * C3DModel::load(const wchar_t * filename)
{
	C3DModel* newObject = nullptr;
	
	// Check the file type
	wstring stdFilename(filename);
	size_t dotIndex = stdFilename.rfind('.', stdFilename.length());
	if (dotIndex != string::npos)
	{
		std::wstring fileExtension = stdFilename.substr(dotIndex + 1, stdFilename.length() - dotIndex);

		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

		if (fileExtension == L"obj") newObject = new C3DModel_Obj();
		else if (fileExtension == L"3ds") newObject = new C3DModel_3DS();
		else
		{
			Log << "ERROR: Unsupported file type" << endl;
			return nullptr;
		}
	}
	else
	{
		Log << "ERROR: Cannot determine the file type" << endl;
		return nullptr;
	}

	newObject->loadFromFile((wchar_t *)filename);
	return newObject;
}

std::string C3DModel::toString()
{
	if (!m_Initialized) return "";

	string res = "";

	res.append("Vertices:\n\n");

	for (int i = 0; i < m_numVertices; i++)
	{
		res.append(to_string(m_vertices[i].getX()));
		res.append(" ");
		res.append(to_string(m_vertices[i].getY()));
		res.append(" ");
		res.append(to_string(m_vertices[i].getZ()));
		res.append("\n");
	}

	res.append("\n");

	res.append("Vertex Normals:\n\n");

	for (int i = 0; i < m_numNormals; i++)
	{
		res.append(to_string(m_normals[i].getX()));
		res.append(" ");
		res.append(to_string(m_normals[i].getY()));
		res.append(" ");
		res.append(to_string(m_normals[i].getZ()));
		res.append("\n");
	}

	res.append("\n");

	res.append("Vertex Textures:\n\n");

	for (int i = 0; i < m_numUVCoords; i++)
	{
		res.append(to_string(m_UVCoords[i].getX()));
		res.append(" ");
		res.append(to_string(m_UVCoords[i].getY()));
		res.append("\n");
	}

	res.append("\n");

	res.append("Faces:\n\n");

	int _numIndices = m_numFaces * 3;
	for (int i = 0; i < _numIndices; i++)
	{
		res.append(to_string(m_vertexIndices[i]));
		res.append("/");
		res.append(to_string(m_UVindices[i]));
		res.append("/");
		//res.append(to_string(m_normalIndices[i]));

		res.append(" ");

		i += 1;

		res.append(to_string(m_vertexIndices[i]));
		res.append("/");
		res.append(to_string(m_UVindices[i]));
		res.append("/");
		//res.append(to_string(m_normalIndices[i]));

		res.append(" ");

		i += 1;

		res.append(to_string(m_vertexIndices[i]));
		res.append("/");
		res.append(to_string(m_UVindices[i]));
		res.append("/");
		//res.append(to_string(m_normalIndices[i]));

		res.append("\n");
	}

	return res;
}
