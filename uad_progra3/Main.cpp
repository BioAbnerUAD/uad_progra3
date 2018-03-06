// uad_progra3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Include/CGameWindow.h"
#include "Include/CApp.h"
#include "Include/CProyectoFinal.h"
#include "Include/CLogger.h"

int main(int argc, char** argv)
{
	/*for (size_t i = 0; i < argc; i++)
		std::cout << argv[i] << std::endl;*/

	CApp *app = NULL;                  // Pointer to BASE class CApp
	app = new CProyectoFinal(800, 600);  // Using pointer to base class, create a new object of DERIVED class CAppParcial2
	app->run();                        // Run the app
	delete app;                        // Delete pointer
	app = NULL;                        // Set pointer to NULL

	return 0;
}

