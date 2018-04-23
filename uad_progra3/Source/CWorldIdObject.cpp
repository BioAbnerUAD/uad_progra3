#include "../Include/CWorldIdObject.h"


CWorldIdObject::CWorldIdObject(int ID)
{
	this->ID = ID;

	CVector3 vertexPositions[] =
	{
		{ -1.0f,  1.0f, -1.0f },  // -x, +y, -z TOP LEFT, BACK      #0
		{ -1.0f,  1.0f,  1.0f },  // -x, +y, +z TOP LEFT, FRONT     #1

		{ 1.0f,  1.0f, -1.0f },  // +x, +y, -z TOP RIGHT, BACK     #2
		{ 1.0f,  1.0f,  1.0f },  // +x, +y, +z TOP RIGHT, FRONT    #3

		{ -1.0f, -1.0f, -1.0f },  // -x, -y, -z BOTTOM LEFT, BACK   #4
		{ -1.0f, -1.0f,  1.0f },  // -x, -y, +z BOTTOM LEFT, FRONT  #5

		{ 1.0f, -1.0f, -1.0f },  // +x, -y, -z BOTTOM RIGHT, BACK  #6
		{ 1.0f, -1.0f,  1.0f },  // +x, -y, +z BOTTOM RIGHT, FRONT #7

							 // DUPLICATE VERTICES
							 // -------------------
							{ -1.0f, -1.0f, -1.0f },  // -x, -y, -z BOTTOM LEFT, BACK   #8
							{ 1.0f, -1.0f, -1.0f },  // +x, -y, -z BOTTOM RIGHT, BACK  #9

							{ -1.0f, -1.0f,  1.0f },  // -x, -y, +z BOTTOM LEFT, FRONT  #10
							{ 1.0f, -1.0f,  1.0f },  // +x, -y, +z BOTTOM RIGHT, FRONT #11

							{ -1.0f, -1.0f, -1.0f },  // -x, -y, -z BOTTOM LEFT, BACK   #12
							{ -1.0f, -1.0f,  1.0f }   // -x, -y, +z BOTTOM LEFT, FRONT  #13
	};

	CVector3 vertexUVs[] =
	{
		{ 0.25f, 0.33f, 0.f }, // TOP LEFT, BACK
		{ 0.25f, 0.66f, 0.f }, // TOP LEFT, FRONT
		{ 0.50f, 0.33f, 0.f }, // TOP RIGHT, BACK
		{ 0.50f, 0.66f, 0.f }, // TOP RIGHT, FRONT
		{ 0.00f, 0.33f, 0.f }, // BOTTOM LEFT, BACK 
		{ 0.00f, 0.66f, 0.f }, // BOTTOM LEFT, FRONT
		{ 0.75f, 0.33f, 0.f }, // BOTTOM RIGHT, BACK
		{ 0.75f, 0.66f, 0.f }, // BOTTOM RIGHT, FRONT

					  // DUPLICATES
					  // ----------
					{ 0.25f, 0.0f, 0.f },  // BOTTOM LEFT, BACK
					{ 0.50f, 0.0f, 0.f },  // BOTTOM RIGHT, BACK
					{ 0.25f, 1.0f, 0.f },  // BOTTOM LEFT, FRONT
					{ 0.50f, 1.0f, 0.f },  // BOTTOM RIGHT, FRONT
					{ 1.0f,  0.33f, 0.f }, // BOTTOM LEFT, BACK
					{ 1.0f,  0.66f, 0.f }  // BOTTOM LEFT, FRONT
	};

	unsigned short indices[] =
	{
		0, 1, 2, // TOP #1
		2, 1, 3, // TOP #2

		0, 4, 1, // LEFT #1
		1, 4, 5, // LEFT #2

		3, 7, 2, // RIGHT #1
		2, 7, 6, // RIGHT #2

		2, 9, 0, // BACK #1
		0, 9, 8, // BACK #2

		1, 10, 3,// FRONT #1
		3, 10, 11,//FRONT #2

		13, 12, 7,//BOTTOM #1
		7, 12, 6  //BOTTOM #2
	};


	m_numVertices = 14;
	m_numFaces = 12;

	memcpy(m_vertices, vertexPositions, 14 * sizeof(CVector3));
	memcpy(m_UVs, vertexUVs, 14 * sizeof(CVector3));
	memcpy(m_indices, indices, 36 * sizeof(unsigned short));
}


CWorldIdObject::~CWorldIdObject()
{
}
