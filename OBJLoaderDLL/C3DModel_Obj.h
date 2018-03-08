#pragma once

#ifdef MAKEDLL
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __declspec(dllimport)
#endif

#ifndef C3DMODEL_OBJ_H
#define C3DMODEL_OBJ_H

#include <string>
using namespace std;

#include "C3DModel.h"
#include "CVector3.h"

class EXPORT C3DModel_Obj : public C3DModel
{
private:
	// Methods to read a model in the .obj file format
	bool readFile(const char * const filename);				// Read object from file
	void reset();											// Cleanup any allocated memory
	bool isInteger(const string &s, unsigned int* outInt);

public:
	C3DModel_Obj();
	~C3DModel_Obj();
};

#endif // !C3DMODEL_OBJ_H
