// FileParser3DS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "3dsFile.h"
#include <iostream>
#include <conio.h>

int main(int argc, char **argv)
{

	C3dsFile* file = new C3dsFile("C:\\Users\\S-55\\Desktop\\Progra3\\LowTree\\Low Tree.3DS");
	cout << file->toString() << endl;
	_getch();

	return 0;
}
