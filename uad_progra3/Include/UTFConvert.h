#pragma once
#ifndef UTFCONVERT_H
#define UTFCONVERT_H

#include <Windows.h>

namespace UTFConvert
{
	static wchar_t* convert_UTF8_UTF16(char* szUTF8)
	{
		if ((szUTF8 == NULL) || (*szUTF8 == '\0')) return new wchar_t(L'\0');

		int size = (int)strlen(szUTF8);

		int new_size = MultiByteToWideChar(CP_UTF8, 0, szUTF8, size, NULL, 0);
		wchar_t* wtext = new wchar_t[new_size];
		MultiByteToWideChar(CP_UTF8, 0, szUTF8, size, wtext, new_size);
		wtext[new_size] = L'\0';

		return wtext;
	}

	static char* convert_UTF16_UTF8(wchar_t* szUTF16)
	{
		if ((szUTF16 == NULL) || (*szUTF16 == L'\0')) return new char('\0');

		int size = (int)wcslen(szUTF16);

		int new_size = WideCharToMultiByte(CP_UTF8, 0, szUTF16, size, NULL, 0, NULL, NULL);
		char* text = new char[new_size];
		WideCharToMultiByte(CP_UTF8, 0, szUTF16, size, text, new_size, NULL, NULL);
		text[new_size] = '\0';

		return text;
	}
}
#endif // !UTFCONVERT_H
