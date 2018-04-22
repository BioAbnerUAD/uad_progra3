#include "../Include/CLogger.h"
#include "../Include/CFileLogger.h"
#include "../Include/CConsoleLogger.h"
#include <Windows.h>

CLogger* CLogger::instance = nullptr;

CLogger * CLogger::GetInstance()
{
	if (!instance)
	{
		char* buff;
		if (buff = getenv("PROGRA3_FILE_LOG"))
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

std::ostream & CLogger::Out()
{
	return *_out;
}
