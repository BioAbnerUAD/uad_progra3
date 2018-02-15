#include "../Include/CLogger.h"
#include "../Include/CFileLogger.h"
#include "../Include/CConsoleLogger.h"
#include <Windows.h>

CLogger* CLogger::instance = nullptr;

CLogger * CLogger::GetInstance()
{
	if (!instance)
	{
		if (getenv("PROGRA3_FILE_LOG") != nullptr)
			CLogger::instance = CFileLogger::GetInstance();
		else
			CLogger::instance = CConsoleLogger::GetInstance();
	}
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
