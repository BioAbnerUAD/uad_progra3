// uad_progra3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Include/CGameWindow.h"
#include "Include/CApp.h"
#include "Include/CProyectoFinal.h"
#include "Include/CAppParcial2.h"
#include "Include/CLogger.h"

int main(int argc, char** argv)
{
	CApp *app = NULL;                  // Pointer to BASE class CApp
	//app = new CAppParcial2(800, 600);
	app = new CProyectoFinal(800, 600);
	app->run();                        // Run the app
	delete app;                        // Delete pointer
	app = NULL;                        // Set pointer to NULL

	return 0;
}

