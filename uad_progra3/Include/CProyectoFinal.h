#pragma once

#ifndef CPROYECTOFINAL_H
#define CPROYECTOFINAL_H

#include "Globals.h"
#include "CApp.h"
#include "C3DModel.h"
#include "CVector3.h"
#include "../CWorld.h"

// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
class CProyectoFinal : public CApp
{
private:
	// Pointer to an object of type C3DModel
	CWorld * m_pWorld;

	// Current delta time (time of the current frame - time of the last frame)
	double m_currentDeltaTime;

protected:
	// Method to initialize the menu
	bool initializeMenu();

public:
	// Constructors and destructor
	CProyectoFinal();
	CProyectoFinal(int window_width, int window_height);
	~CProyectoFinal();

	// Inherited methods from CApp
	// ---------------------------

	// Method to update any objecs based on time elapsed since last frame
	void update(double deltaTime);

	// Method to run the app
	void run();

	// Method to render the app
	void render();

	// Executes the action corresponding to the selected menu item
	void executeMenuAction();

	// Methods specific to CAppParcial2 only
	// -------------------------------------

	// Derived class key input responces
	void onF2(int mods);
	void onF3(int mods);
	void onF5(int mods);
	void onArrowUp(int mods);
	void onArrowDown(int mods);
	void onArrowLeft(int mods);
	void onArrowRight(int mods);
	void onMouseMove(float dx, float dy);
	void onSpaceBar(int mods);
	void onCKey(int mods);
	void onMouseLeftClick();

private:

	// Load/unload 3D model
	bool loadWorld();
	void unloadWorld();

	// Move camera away/closer
	void moveCamera(float direction);
};

#endif // !CPROYECTOFINAL_H