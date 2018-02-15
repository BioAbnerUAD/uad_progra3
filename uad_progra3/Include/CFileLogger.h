#pragma once
#include "CLogger.h"
#include <fstream>
class CFileLogger : public CLogger
{
	CFileLogger();
	static CFileLogger* instance;
	std::fstream outFile;
public:
	static CFileLogger* GetInstance();
	static void ResetInstance();
	~CFileLogger();
};

