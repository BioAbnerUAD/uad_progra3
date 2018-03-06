#include "../stdafx.h"

#include "../Include/CLogger.h"

#include "../Include/CApp.h"

#include <VersionHelpers.h>

CCamera * const CApp::getCamera()
{
	return m_Camera;
}

/* */
CApp::CApp()
{
	Log << "Constructor: CApp()" << endl;

	// Create OpenGLRenderer Object
	m_OpenGLRenderer = new COpenGLRenderer();

	// Create CGameWindow object
	m_Window = new CGameWindow(m_OpenGLRenderer);

	// Create CGameMenu Object
	// Menu will be initialized later (menu items added)
	m_Menu = new CGameMenu();

	m_Camera = new CCamera();

	if (IsWindows10OrGreater()) Log << "Windows 10 detected" << endl;
	else if (IsWindows8Point1OrGreater()) Log << "Windows 8.1 detected" << endl;
	else if (IsWindows8OrGreater()) Log << "Windows 8 detected (Might as well kill yourself)" << endl;
	else if (IsWindows7OrGreater()) Log << "Windows 7 detected" << endl;
	else if (IsWindowsVistaOrGreater()) Log << "Windows Vista detected (PLEASE KILL YOURSELF)" << endl;
	else if (IsWindowsXPOrGreater()) Log << "Windows XP detected" << endl;
}

/* */
CApp::CApp(int window_width, int window_height)
{
	Log << "Constructor: CApp(int window_width, int window_height)" << endl;
	
	// Create OpenGLRenderer Object
	m_OpenGLRenderer = new COpenGLRenderer();

	// Create CGameWindow object
	m_Window = new CGameWindow(m_OpenGLRenderer, window_width, window_height);

	// Create CGameMenu Object
	// Menu will be initialized later (menu items added)
	m_Menu = new CGameMenu();

	m_Camera = new CCamera();

	if (IsWindows10OrGreater()) Log << "Windows 10 detected" << endl;
	else if (IsWindows8Point1OrGreater()) Log << "Windows 8.1 detected" << endl;
	else if (IsWindows8OrGreater()) Log << "Windows 8 detected (Might as well kill yourself)" << endl;
	else if (IsWindows7OrGreater()) Log << "Windows 7 detected" << endl;
	else if (IsWindowsVistaOrGreater()) Log << "Windows Vista detected (PLEASE KILL YOURSELF)" << endl;
	else if (IsWindowsXPOrGreater()) Log << "Windows XP detected" << endl;
}

/* */
CApp::~CApp()
{
	Log << "Destructor: ~CApp()" << endl;

	// Free CGameWindow object resources
	if (m_Window != NULL)
	{
		Log << "Delete m_Window" << endl;
		delete m_Window;
	}

	m_Window = NULL;

	// Free CGameMenu object resources
	if (m_Menu != NULL)
	{
		// Cleanup call deleteVertexArrayObject
		m_Menu->cleanupGraphicsObjects(m_OpenGLRenderer);

		Log << "Delete m_Menu" << endl;
		delete m_Menu;
	}

	m_Menu = NULL;

	// Free COpenGLRenderer object resources
	if (m_OpenGLRenderer != NULL)
	{
		Log << "Delete m_OpenGLRenderer" << endl;
		delete m_OpenGLRenderer;
	}

	m_OpenGLRenderer = NULL;
}

/* */
bool CApp::isWindowInitialized() const
{
	if (m_Window != NULL)
	{
		return m_Window->isLibraryInitialized();
	}

	return false;
}

/* */
bool CApp::canRun() const
{
	return isWindowInitialized();
}

/* */
bool CApp::isMenuActive() const
{
	if (m_Menu != NULL)
	{
		return m_Menu->isActive();
	}

	return false;
}

/* */
void CApp::setMenuActive(bool active)
{
	if (m_Menu != NULL)
	{
		m_Menu->setActive(active);

		if (active)
		{
			if (m_OpenGLRenderer != NULL)
			{
				m_OpenGLRenderer->setClearScreenColor(0.88f, 0.88f, 0.88f);
			}

			Log << "Menu is active" << endl;
			Log << "Selected option is #" << m_Menu->getSelectedMenuItemNumber() << endl;
		}
		else
		{
			if (m_OpenGLRenderer != NULL)
			{
				m_OpenGLRenderer->setClearScreenColor(0.15f, 0.75f, 0.75f);
			}

			Log << "Menu is NOT active" << endl;
		}
	}
}

/* */
void CApp::executeMenuAction()
{
	if (m_Menu != NULL)
	{
		Log << "Execute Menu action" << endl;
	}
}

/* */
void CApp::selectNextMenuItem()
{
	if (m_Menu != NULL)
	{
		m_Menu->selectMenuItem(true);
		Log << "Select next menu option" << endl;
		Log << "Selected option is #" << m_Menu->getSelectedMenuItemNumber() << endl;
	}
}

/* */
void CApp::selectPrevMenuItem()
{
	if (m_Menu != NULL)
	{
		m_Menu->selectMenuItem(false);
		Log << "Select prev menu option" << endl;
		Log << "Selected option is #" << m_Menu->getSelectedMenuItemNumber() << endl;
	}
}

