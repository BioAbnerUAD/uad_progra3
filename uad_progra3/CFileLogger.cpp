#include "CFileLogger.h"
#include <windows.h>
#include <ctime>


std::string getCurrentTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y_%I:%M:%S", timeinfo);
	std::string str(buffer);

	return str;
}

CFileLogger::CFileLogger()
{
	std::string folder;
	_out = &outFile;
	folder = getenv("PROGRA_TEMP");
	if (folder.empty())
		folder = getenv("TEMP");
	if (folder.empty())
		folder = "/tmp";

	folder += "uad_progra3_";
	folder += getCurrentTime();
	folder += ".log";

	outFile.open(folder.c_str(), std::ios::out);
}


CFileLogger::~CFileLogger()
{
	outFile.close();
}
