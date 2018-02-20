#include "../Include/CFileLogger.h"
#include <windows.h>
#include <ctime>

CFileLogger* CFileLogger::instance = nullptr;

std::string getCurrentTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", timeinfo);
	std::string str(buffer);

	return str;
}

std::string getEXEName()
{
	char pathBuff[MAX_PATH];
	GetModuleFileNameA(0, pathBuff, MAX_PATH);
	
	std::string path(pathBuff);
	size_t i_start = path.find_last_of("\\") + 1;
	std::string name = path.substr(i_start, path.find_first_of(".") - i_start);

	return name;
}

CFileLogger::CFileLogger()
{
	std::string folder;
	_out = &outFile;

	if(getenv("PROGRA_TEMP")) folder = getenv("PROGRA_TEMP");
	else if (getenv("TEMP")) folder = getenv("TEMP");
	else throw new std::runtime_error("Temp folder enviroment vairable not found.");

	folder += "\\";
	folder += getEXEName();
	folder += "_";

	folder += getCurrentTime();
	folder += ".log";

	outFile.open(folder.c_str(), std::ios::out);
}


CFileLogger * CFileLogger::GetInstance()
{
	if (!instance) instance = new CFileLogger();
	return instance;
}

void CFileLogger::ResetInstance()
{
	delete instance;
	instance = nullptr;
}

CFileLogger::~CFileLogger()
{
	outFile.close();
}
