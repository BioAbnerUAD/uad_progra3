#include "../Include/CConsoleLogger.h"

CConsoleLogger* CConsoleLogger::instance = nullptr;

CConsoleLogger::CConsoleLogger()
{
	_out = &std::cout;
}


CConsoleLogger * CConsoleLogger::GetInstance()
{
	if (!instance) instance = new CConsoleLogger();
	return instance;
}

void CConsoleLogger::ResetInstance()
{
	delete instance;
	instance = nullptr;
}

CConsoleLogger::~CConsoleLogger()
{
}
