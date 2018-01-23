#include "stdafx.h"
#include "3dsFile.h"

using namespace std;

C3dsFile::C3dsFile()
{
	cout << "Constructor : C3dsFile() " << endl;
	m_vertexIndices = m_normalIndices = m_UVindices = nullptr;
	m_vertices = m_normals = m_UVCoords = nullptr;
	/*m_shaderProgramId = m_graphicsMemoryObjectId = */m_numVertices
		= m_numNormals = m_numUVCoords = m_numFaces = m_currentVertex
		= m_currentNormal = m_currentUV = m_currentFace = m_Initialized = 0;
}

C3dsFile::C3dsFile(string file)
{
	cout << "Constructor : C3dsFile() " << endl;
	this->Build3DS(file);
}

C3dsFile::C3dsFile(const C3dsFile & obj)
{
	cout << "Constructor : C3dsFile() " << endl;

	this->m_vertexIndices = new unsigned short[obj.m_numFaces * 3];
	memcpy(this->m_vertexIndices, obj.m_vertexIndices, obj.m_numFaces * 3);

	this->m_normalIndices = new unsigned short[obj.m_numFaces * 3];
	memcpy(this->m_normalIndices, obj.m_normalIndices, obj.m_numFaces * 3);

	this->m_UVindices = new unsigned short[obj.m_numFaces * 3];
	memcpy(this->m_UVindices, obj.m_UVindices, obj.m_numFaces * 3);

	this->m_UVCoords = new float[obj.m_numUVCoords];
	memcpy(this->m_UVCoords, obj.m_UVCoords, obj.m_numUVCoords);

	this->m_normals = new float[obj.m_numNormals];
	memcpy(this->m_normals, obj.m_normals, obj.m_numNormals);

	this->m_vertices = new float[obj.m_numVertices];
	memcpy(this->m_vertices, obj.m_vertices, obj.m_numVertices);

	m_numVertices = obj.m_numVertices;
	m_numUVCoords = obj.m_numUVCoords;
	m_numFaces = obj.m_numFaces;
	m_currentVertex = obj.m_currentVertex;
	m_currentNormal = obj.m_currentNormal;
	m_currentUV = obj.m_currentUV;
	m_currentFace = obj.m_currentFace;

	m_Initialized = obj.m_Initialized;
}

C3dsFile::~C3dsFile()
{
	cout << "Destructor : C3dsFile() " << endl;
	reset();
}

bool C3dsFile::Build3DS(string file)
{
	if (isInitialized()) reset();

	bin3ds.open(file, ios::in | ios::binary);

	if (!bin3ds.is_open())
		return false;
	dinObj = new DinamicOBJ();
	bool res = ReadPrimaryChunk();
	bin3ds.close();

	this->m_vertexIndices = new unsigned short[dinObj->indices.size()];
	memcpy(this->m_vertexIndices, &(dinObj->indices)[0], dinObj->indices.size() * sizeof(unsigned short));

	this->m_normalIndices = new unsigned short[dinObj->indicesNorm.size()];
	memcpy(this->m_normalIndices, &(dinObj->indicesNorm)[0], dinObj->indicesNorm.size() * sizeof(unsigned short));

	this->m_UVindices = new unsigned short[dinObj->indicesUVs.size()];
	memcpy(this->m_UVindices, &(dinObj->indicesUVs)[0], dinObj->indicesUVs.size() * sizeof(unsigned short));

	this->m_vertices = new float[dinObj->vertices.size()];
	memcpy(this->m_vertices, &(dinObj->vertices)[0], dinObj->vertices.size() * sizeof(float));

	this->m_normals = new float[dinObj->normals.size()];
	memcpy(this->m_normals, &(dinObj->normals)[0], dinObj->normals.size() * sizeof(float));

	this->m_UVCoords = new float[dinObj->UVcoords.size()];
	memcpy(this->m_UVCoords, &(dinObj->UVcoords)[0], dinObj->UVcoords.size() * sizeof(float));

	this->m_numFaces = dinObj->indices.size() / 3;
	this->m_numVertices = dinObj->vertices.size();
	this->m_numNormals = dinObj->normals.size();
	this->m_numUVCoords = dinObj->UVcoords.size();

	m_Initialized = true;
	delete dinObj;
	return true;
}

void C3dsFile::reset()
{
	if (m_vertexIndices != nullptr)
	{
		delete[] m_vertexIndices;
		m_vertexIndices = nullptr;
	}
	if (m_normalIndices != nullptr)
	{
		delete[] m_normalIndices;
		m_normalIndices = nullptr;
	}
	if (m_UVindices != nullptr)
	{
		delete[] m_UVindices;
		m_UVindices = nullptr;
	}
	if (m_vertices != nullptr)
	{
		delete[] m_vertices;
		m_vertices = nullptr;
	}
	if (m_normals != nullptr)
	{
		delete[] m_normals;
		m_normals = nullptr;
	}
	if (m_UVCoords != nullptr)
	{
		delete[] m_UVCoords;
		m_UVCoords = nullptr;
	}

	/*m_shaderProgramId = m_graphicsMemoryObjectId = */m_numVertices
		= m_numNormals = m_numUVCoords = m_numFaces
		= m_currentVertex = m_currentNormal = m_currentUV
		= m_currentFace = m_Initialized = 0;
}

string C3dsFile::toString()
{
	if (!m_Initialized) return "";

	string res = "";

	res.append("Vertices:\n\n");

	for (int i = 0; i < m_numVertices; i += 3)
	{
		res.append(to_string(m_vertices[i]));
		res.append(" ");
		res.append(to_string(m_vertices[i + 1]));
		res.append(" ");
		res.append(to_string(m_vertices[i + 2]));
		res.append("\n");
	}

	res.append("\n");

	res.append("Vertex Normals:\n\n");

	for (int i = 0; i < m_numNormals; i += 3)
	{
		res.append(to_string(m_normals[i]));
		res.append(" ");
		res.append(to_string(m_normals[i + 1]));
		res.append(" ");
		res.append(to_string(m_normals[i + 2]));
		res.append("\n");
	}

	res.append("\n");

	res.append("Vertex Textures:\n\n");

	for (int i = 0; i < m_numUVCoords; i += 2)
	{
		res.append(to_string(m_UVCoords[i]));
		res.append(" ");
		res.append(to_string(m_UVCoords[i + 1]));
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
		res.append(to_string(m_normalIndices[i]));

		res.append(" ");

		i += 1;

		res.append(to_string(m_vertexIndices[i]));
		res.append("/");
		res.append(to_string(m_UVindices[i]));
		res.append("/");
		res.append(to_string(m_normalIndices[i]));

		res.append(" ");

		i += 1;

		res.append(to_string(m_vertexIndices[i]));
		res.append("/");
		res.append(to_string(m_UVindices[i]));
		res.append("/");
		res.append(to_string(m_normalIndices[i]));

		res.append("\n");
	}

	return res;
}

unsigned char C3dsFile::ReadChar()
{
	char c;
	bin3ds.read(&c, 1);
	return c;
}

unsigned short C3dsFile::ReadInt()
{
	unsigned short temp = ReadChar();
	return (temp | (ReadChar() << 8));
}

unsigned long C3dsFile::ReadLong()
{
	unsigned long temp1, temp2;

	temp1 = ReadInt();
	temp2 = ReadInt();

	return (temp1 + (temp2 * 0x10000L));
}

float C3dsFile::ReadFloat()
{
	float f;
	bin3ds.read(reinterpret_cast<char*>(&f), sizeof(float));
	return f;
}

unsigned long C3dsFile::ReadChunkPointer()
{
	return ReadLong();
}

unsigned long C3dsFile::GetChunkPointer()
{
	return ((unsigned long)bin3ds.tellg() - 2); // compensate for the already read Marker
}

void C3dsFile::ChangeChunkPointer(unsigned long temp_pointer)
{
	bin3ds.seekg(temp_pointer, ios::beg);
}

int C3dsFile::ReadName(bool isLongName)
{
	unsigned char letter;

	temp_name.clear();

	do
	{
		letter = ReadChar();
		temp_name += letter;
	} while ((letter != 0) && (temp_name.size() < 12 || isLongName));

	if (temp_name == "")
	{
		temp_name = "Default name";
		return -1;
	}

	cout << "     Found name : " << temp_name << endl;
	return 0;
}

unsigned long C3dsFile::ReadChunk(unsigned int chunk_id)
{
	unsigned int temp_int;
	unsigned long current_pointer;
	unsigned long chunk_size;//temp_pointer
	unsigned long counter = 6L;

	current_pointer = GetChunkPointer();
	chunk_size = ReadChunkPointer();

	if (chunk_id == EDIT_OBJECT) ReadName(false);
	if (chunk_id == TRI_VERTEXL)
	{
		unsigned int numb_vertices = ReadInt();
		cout << "      Found (" << numb_vertices << ") number of vertices" << endl;

		for (size_t i = 0; i < numb_vertices; i++)
		{
			dinObj->vertices.push_back(ReadFloat());
			dinObj->vertices.push_back(ReadFloat());
			dinObj->vertices.push_back(ReadFloat());
		}

		counter = chunk_size;
	}
	if (chunk_id == TRI_FACEL1)
	{
		unsigned int numb_faces = ReadInt();
		cout << "      Found (" << numb_faces << ") number of faces" << endl;

		for (size_t i = 0; i < numb_faces; i++)
		{
			dinObj->indices.push_back(ReadInt());
			dinObj->indices.push_back(ReadInt());
			dinObj->indices.push_back(ReadInt());
			ReadInt();
		}

		if (ReadInt() == TRI_SMOOTH)
			ReadChunk(TRI_SMOOTH); // TODO: ReadSmoothingChunk

		counter = chunk_size;
	}
	if (chunk_id == TRI_MAPPINGCOORS)
	{
		unsigned short numVertices = ReadInt();
		cout << "      Found (" << numVertices << ") number of uv vertices" << endl;
		for (size_t i = 0; i < numVertices; i++)
		{
			dinObj->UVcoords.push_back(ReadFloat());
			dinObj->UVcoords.push_back(ReadFloat());
		}
		counter = chunk_size;
	}
	

	while (counter < chunk_size)
	{
		temp_int = ReadInt();
		switch (chunk_id)
		{
		case MAIN3DS: //ReadMainChunk
			switch (temp_int)
			{
			case KEYF3DS:
				cout << ">> Found *Keyframer* chunk id of " << std::hex << KEYF3DS << std::dec << endl;
				counter += ReadChunk(KEYF3DS);
				break;
			case EDIT3DS:
				cout << ">> Found *Editor* chunk id of " << std::hex << EDIT3DS << std::dec << endl;
				counter += ReadChunk(EDIT3DS);
				break;
			default: break;
			}
			break;
		case EDIT3DS://ReadEditChunk
			switch (temp_int)
			{
			case EDIT_UNKNW01:counter += ReadChunk(EDIT_UNKNW01); break;
			case EDIT_UNKNW02:counter += ReadChunk(EDIT_UNKNW02); break;
			case EDIT_UNKNW03:counter += ReadChunk(EDIT_UNKNW03); break;
			case EDIT_UNKNW04:counter += ReadChunk(EDIT_UNKNW04); break;
			case EDIT_UNKNW05:counter += ReadChunk(EDIT_UNKNW05); break;
			case EDIT_UNKNW06:counter += ReadChunk(EDIT_UNKNW06); break;
			case EDIT_UNKNW07:counter += ReadChunk(EDIT_UNKNW07); break;
			case EDIT_UNKNW08:counter += ReadChunk(EDIT_UNKNW08); break;
			case EDIT_UNKNW09:counter += ReadChunk(EDIT_UNKNW09); break;
			case EDIT_UNKNW10:counter += ReadChunk(EDIT_UNKNW10); break;
			case EDIT_UNKNW11:counter += ReadChunk(EDIT_UNKNW11); break;
			case EDIT_UNKNW12:counter += ReadChunk(EDIT_UNKNW12); break;
			case EDIT_UNKNW13:counter += ReadChunk(EDIT_UNKNW13); break;
			case EDIT_MATERIAL: counter += ReadChunk(EDIT_MATERIAL); break;
			case EDIT_VIEW1: counter += ReadChunk(EDIT_VIEW1); break;
			case EDIT_BACKGR: counter += ReadChunk(EDIT_BACKGR); break;
			case EDIT_AMBIENT: counter += ReadChunk(EDIT_AMBIENT); break;
			case EDIT_OBJECT: counter += ReadChunk(EDIT_OBJECT); break;
			default: break;
			}
			break;
		case EDIT_OBJECT://ReadObjectChunk
			switch (temp_int)
			{
			case OBJ_UNKNWN01:counter += ReadChunk(OBJ_UNKNWN01); break;
			case OBJ_UNKNWN02:counter += ReadChunk(OBJ_UNKNWN02); break;
			case OBJ_TRIMESH:
				cout << ">>>> Found Obj/Mesh chunk id of " << std::hex << OBJ_TRIMESH << std::dec << endl;
				counter += ReadChunk(OBJ_TRIMESH);
				break;
			case OBJ_LIGHT: counter += ReadChunk(OBJ_LIGHT); break;
			case OBJ_CAMERA: counter += ReadChunk(OBJ_CAMERA); break;
			default:          break;
			}
			break;
		case OBJ_TRIMESH://ReadObjChunk
			switch (temp_int)
			{
			case TRI_VERTEXL:
				cout << ">>>> Found Object vertices chunk id of " << std::hex << TRI_VERTEXL << std::dec << endl;
				counter += ReadChunk(TRI_VERTEXL);
				break;
			case TRI_FACEL1:
				cout << ">>>> Found Object faces (1) chunk id of " << std::hex << TRI_FACEL1 << std::dec << endl;
				counter += ReadChunk(TRI_FACEL1);
				break;
			case TRI_MAPPINGCOORS:
				cout << ">>>> Found Mapping Coordinates chunk id of " << std::hex << TRI_MAPPINGCOORS << std::dec << endl;
				counter += ReadChunk(TRI_MAPPINGCOORS);
				break;

			case TRI_FACEL2: counter += ReadChunk(TRI_FACEL2); break;
			case TRI_LOCAL: counter += ReadChunk(TRI_LOCAL); break;
			case TRI_VISIBLE: counter += ReadChunk(TRI_VISIBLE); break;
			default: break;
			}
			break;
		default:
			counter = chunk_size;
			break;
		}

		counter += 2;
	}

	ChangeChunkPointer(current_pointer + chunk_size);
	// move to the new chunk position
	return chunk_size;
}

bool C3dsFile::ReadPrimaryChunk()
{
	unsigned char version;

	if (ReadInt() == MAIN3DS)
	{
		cout << "> Found Main chunk id of " << std::hex << MAIN3DS << std::dec << endl;
		bin3ds.seekg(28L, ios::beg);
		version = ReadChar();
		if (version < 3)
		{
			cout << "Sorry this lib can only read 3ds files of version 3.0 and higher" << endl;
			cout << "The version of the file you want to read is: "  << version << endl;
			return true;
		}
		bin3ds.seekg(2, ios::beg);
		ReadChunk(MAIN3DS);
	}
	else return true;
	return false;
}
