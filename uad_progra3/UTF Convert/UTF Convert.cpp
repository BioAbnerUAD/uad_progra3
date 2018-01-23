// UTF Convert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <Windows.h>

using namespace std;

int main()
{
    return 0;
}

wstring convert_UTF8_UTF16(char* szUTF8, size_t size)
{
	if ((szUTF8 == NULL) || (*szUTF8 == '\0')) return L"";

	int new_size = MultiByteToWideChar(CP_UTF8, 0, szUTF8, size, NULL, 0);
	wchar_t* wtext = new wchar_t[new_size];
	MultiByteToWideChar(CP_UTF8, 0, szUTF8, size, wtext, new_size);
	
	return wtext;
}

string convert_UTF16_UTF8(wchar_t* szUTF16, size_t size)
{
	if ((szUTF16 == NULL) || (*szUTF16 == L'\0')) return "";

	int new_size = WideCharToMultiByte(CP_UTF8, 0, szUTF16, size, NULL, 0, NULL, NULL);
	char* text = new char[new_size];
	WideCharToMultiByte(CP_UTF8, 0, szUTF16, size, text, new_size, NULL, NULL);

	return text;
}