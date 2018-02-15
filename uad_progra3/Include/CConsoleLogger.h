#pragma once
#include "CLogger.h"
class CConsoleLogger :public CLogger
{
	CConsoleLogger();
	static CConsoleLogger* instance;
public:
	static CConsoleLogger* GetInstance();
	static void ResetInstance();
	~CConsoleLogger();
};

