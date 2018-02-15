#pragma once
#include "CLogger.h"
#include <fstream>
class CFileLogger : public CLogger
{
	CFileLogger();
	std::fstream outFile;
public:
	~CFileLogger();
};

