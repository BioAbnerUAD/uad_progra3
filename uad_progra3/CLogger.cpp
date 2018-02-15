#include "CLogger.h"
#include "CFileLogger.h"
#include "CConsoleLogger.h"
#include <Windows.h>

CLogger* CLogger::instance = nullptr;

CLogger::CLogger()
{
	if (getenv("PROGRA3_FILE_LOG") != nullptr)
		CLogger::instance = CFileLogger::instance;
	else
		CLogger::instance = CConsoleLogger::instance;
	delete this;
}

CLogger * CLogger::GetInstance()
{
	if (!instance) instance = new CLogger();
	return instance;
}

void CLogger::ResetInstance()
{
	delete instance;
	instance = nullptr;
}

std::ostream & const CLogger::Out()
{
	return *_out;
}

CLogger::~CLogger()
{
}
