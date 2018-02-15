#pragma once
#include <iostream>

class CLogger
{
protected:
	static CLogger* instance;
	std::ostream* _out;
public:
	static CLogger* GetInstance();
	static void ResetInstance();
	std::ostream& const Out();
};
