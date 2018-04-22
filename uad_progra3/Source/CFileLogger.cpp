#include "../Include/CFileLogger.h"
#include <windows.h>
#include <ctime>

CFileLogger* CFileLogger::instance = nullptr;

std::string getCurrentTime()
{
	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", &timeinfo);
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
	char* buff;

	if(buff = getenv("PROGRA_TEMP")) folder = buff;
	else if (buff = getenv("TEMP")) folder = buff;
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
