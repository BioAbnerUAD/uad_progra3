#pragma once
#include <iostream>

#define Log CLogger::GetInstance()->Out()

class CLogger
{
protected:
	static CLogger* instance;
	std::ostream* _out;
public:
	static CLogger* GetInstance();
	static void ResetInstance();
	std::ostream& Out();
};
