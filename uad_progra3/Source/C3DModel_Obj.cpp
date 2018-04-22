#include "../stdafx.h"
#include "../Include/CLogger.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "../Include/C3DModel_Obj.h"

/**/
C3DModel_Obj::C3DModel_Obj() : C3DModel()
{
	Log << "Constructor: C3DModel_Obj()" << endl;
}

/**/
C3DModel_Obj::~C3DModel_Obj()
{
	Log << "Destructor: C3DModel_Obj()" << endl;
	reset();
}

/**/
bool C3DModel_Obj::readFile(const char * const filename)
{
	vector<unsigned short> indices(0);
	vector<unsigned short> indicesNorm(0);
	vector<unsigned short> indicesUVs(0);
	vector<float> verticesR(0);
	vector<float> normalsR(0);
	vector<float> UVcoordsR(0);
	vector<CVector3> vertices(0);
	vector<CVector3> normals(0);
	vector<CVector3> UVcoords(0);

	string line;

	ifstream myfile(filename);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			istringstream iss(line);
			string token;
			if (getline(iss, token, ' '))
			{
				if (token == "v")
				{
					CVector3 vertex;
					do {
						getline(iss, token, ' ');
					} while (token.empty());
					verticesR.push_back((float)atof(token.c_str()));
					getline(iss, token, ' ');
					verticesR.push_back((float)atof(token.c_str()));
					getline(iss, token, ' ');
					verticesR.push_back((float)atof(token.c_str()));

					auto it = verticesR.rbegin();
					vertex.setValues(it[2], it[1], it[0]);
					vertices.push_back(vertex);
				}
				else if (token == "vt")
				{
					CVector3 vertex;
					getline(iss, token, ' ');
					UVcoordsR.push_back((float)atof(token.c_str()));
					getline(iss, token, ' ');
					UVcoordsR.push_back((float)atof(token.c_str()));

					auto it = UVcoordsR.rbegin();
					vertex.setValues(it[1], it[0], -1);
					UVcoords.push_back(vertex);
				}
				else if (token == "vn")
				{
					CVector3 vertex;
					getline(iss, token, ' ');
					normalsR.push_back((float)atof(token.c_str()));
					getline(iss, token, ' ');
					normalsR.push_back((float)atof(token.c_str()));
					getline(iss, token, ' ');
					normalsR.push_back((float)atof(token.c_str()));

					auto it = normalsR.rbegin();
					vertex.setValues(it[2], it[1], it[0]);
					normals.push_back(vertex);
				}
				else if (token == "f")
				{
					for (int i = 0; i < 3; i++)
					{
						unsigned int index;

						getline(iss, token, ' ');
						istringstream iss_f(token);
						string token_f;

						getline(iss_f, token_f, '/');
						index = atoi(token_f.c_str());
						indices.push_back(index - 1);

						if (getline(iss_f, token_f, '/') && isInteger(token_f, &index))
						{
							indicesUVs.push_back(index - 1);
						}
						else indicesUVs.push_back(-1);

						if (getline(iss_f, token_f, '/') && isInteger(token_f, &index))
						{
							indicesNorm.push_back(index - 1);
						}
						else indicesNorm.push_back(-1);
					}
				}
			}
		}
		myfile.close();

		this->m_vertexIndices = new unsigned short[indices.size()];
		memcpy(this->m_vertexIndices, &indices[0], indices.size() * sizeof(unsigned short));

		this->m_normalIndices = new unsigned short[indicesNorm.size()];
		memcpy(this->m_normalIndices, &indicesNorm[0], indicesNorm.size() * sizeof(unsigned short));

		this->m_UVindices = new unsigned short[indicesUVs.size()];
		memcpy(this->m_UVindices, &indicesUVs[0], indicesUVs.size() * sizeof(unsigned short));

		this->m_vertices = new CVector3[vertices.size()];
		memcpy(this->m_vertices, &vertices[0], vertices.size() * sizeof(CVector3));

		this->m_normals = new CVector3[normals.size()];
		memcpy(this->m_normals, &normals[0], normals.size() * sizeof(CVector3));

		this->m_UVCoords = new CVector3[UVcoords.size()];
		memcpy(this->m_UVCoords, &UVcoords[0], UVcoords.size() * sizeof(CVector3));

		this->m_verticesRaw = new float[verticesR.size()];
		memcpy(this->m_verticesRaw, &verticesR[0], verticesR.size() * sizeof(float));

		this->m_normalsRaw = new float[normalsR.size()];
		memcpy(this->m_normalsRaw, &normalsR[0], normalsR.size() * sizeof(float));

		this->m_uvCoordsRaw = new float[UVcoordsR.size()];
		memcpy(this->m_uvCoordsRaw, &UVcoordsR[0], UVcoordsR.size() * sizeof(float));

		this->m_numFaces = (int)indices.size() / 3;
		this->m_numVertices = (int)verticesR.size();
		this->m_numNormals = (int)normalsR.size();
		this->m_numUVCoords = (int)UVcoordsR.size();

		m_Initialized = true;
		//cout << toString() << endl;
		return true;
	}

	return false;
}

/**/
void C3DModel_Obj::reset()
{
	if (m_vertexIndices != NULL)
	{
		Log << "deleting vertex indices" << endl;
		delete[] m_vertexIndices;
		m_vertexIndices = NULL;
	}
	if (m_normalIndices != NULL)
	{
		Log << "deleting normal indices" << endl;
		delete[] m_normalIndices;
		m_normalIndices = NULL;
	}
	if (m_UVindices != NULL)
	{
		Log << "deleting UV indices" << endl;
		delete[] m_UVindices;
		m_UVindices = NULL;
	}
	if (m_vertices != NULL)
	{
		Log << "deleting vertices" << endl;
		delete[] m_vertices;
		m_vertices = NULL;
	}
	if (m_verticesRaw != NULL)
	{
		Log << "deleting vertices (float)" << endl;
		delete[] m_verticesRaw;
		m_verticesRaw = NULL;
	}
	if (m_normals != NULL)
	{
		Log << "deleting normals" << endl;
		delete[] m_normals;
		m_normals = NULL;
	}
	if (m_normalsRaw != NULL)
	{
		Log << "deleting normals (float)" << endl;
		delete[] m_normalsRaw;
		m_normalsRaw = NULL;
	}
	if (m_UVCoords != NULL)
	{
		Log << "deleting UV coords" << endl;
		delete[] m_UVCoords;
		m_UVCoords = NULL;
	}
	if (m_uvCoordsRaw != NULL)
	{
		Log << "deleting uvCoords (float)" << endl;
		delete[] m_uvCoordsRaw;
		m_uvCoordsRaw = NULL;
	}

	m_numVertices = 0;
	m_numNormals = 0;
	m_numUVCoords = 0;
	m_numFaces = 0;

	m_Initialized = false;

	m_currentVertex = 0;
	m_currentNormal = 0;
	m_currentUV = 0;
	m_currentFace = 0;

	m_graphicsMemoryObjectId = 0;
	m_shaderProgramId = 0;
}

/**/
bool C3DModel_Obj::isInteger(const string &s, unsigned int* outInt)
{
	*outInt = 0;
	if (s.empty() || !isdigit(s[0])) return false;
	char* p;
	*outInt = strtol(s.c_str(), &p, 10);
	return (*p == 0);
}
