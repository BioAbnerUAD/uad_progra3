#include "../stdafx.h"

#include "windows.h"

#include <vector>
#include <string>

#include "../Include/Globals.h"
#include "../Include/CProyectoFinal.h"
#include "../Include/C3DModel.h"
#include "../Include/LoadTGA.h"
#include "../Include/CWideStringHelper.h"
#include "../UTFConvert.h"

/* */
CProyectoFinal::CProyectoFinal() :
	m_pWorld(NULL),
	m_currentDeltaTime{ 0.0 },
	m_objectPosition{ 0.0f, 0.0f, 0.0f }
{
	Log << "Constructor: CProyectoFinal()" << endl;
}

/* */
CProyectoFinal::CProyectoFinal(int window_width, int window_height) :
	CApp(window_width, window_height),
	m_pWorld(NULL),
	m_currentDeltaTime{ 0.0 },
	m_objectPosition{ 0.0f, 0.0f, 0.0f }
{
	Log << "Constructor: CProyectoFinal(int window_width, int window_height)" << endl;
}

/* */
CProyectoFinal::~CProyectoFinal()
{
	Log << "Destructor: ~CProyectoFinal()" << endl;
	unloadWorld();
}

/* */
void CProyectoFinal::run()
{
	// Check if CGameWindow object and window library been initialized
	if (canRun())
	{
		// Create the Window 
		if (getGameWindow()->create(CAPPPARCIAL2_WINDOW_TITLE))
		{
			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.15f, 0.75f, 0.75f);
			// Initialize window width/height in the renderer
			getOpenGLRenderer()->setWindowWidth(getGameWindow()->getWidth());
			getOpenGLRenderer()->setWindowHeight(getGameWindow()->getHeight());
			// Initialize a test cube
			getOpenGLRenderer()->initializeTestObjects();

			// Create our menu (add all menu items)
			if (!initializeMenu())
			{
				return;
			}

			// Enter main loop
			Log << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}

/* */
bool CProyectoFinal::initializeMenu()
{
	Log << "CProyectoFinal::initializeMenu()" << endl;

	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;
	std::wstring wresourceFilenameTexture;
	std::string resourceFilenameVS;
	std::string resourceFilenameFS;
	std::string resourceFilenameTexture;

	// If resource files cannot be found, return
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_MENU, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_MENU, wresourceFilenameFS, resourceFilenameFS) ||
		!CWideStringHelper::GetResourceFullPath(MENU_TEXTURE_FILE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		Log << "ERROR: Unable to find one or more resources: " << endl;
		Log << "  " << VERTEX_SHADER_MENU << endl;
		Log << "  " << FRAGMENT_SHADER_MENU << endl;
		Log << "  " << MENU_TEXTURE_FILE << endl;
		return false;
	}

	if (getMenu() != NULL)
	{
		CGameMenu *const menu = getMenu();

		unsigned int menuShaderProgramId = 0;
		unsigned int textureObjectId = 0;
		unsigned int vaoMenuItemId = 0;
		int colorUniformLocation = -1;
		int textureUniformLocation = -1;
		bool generatedMenuItemGeometry = false;
		float currentX = 0.65f, currentY = 0.8f;
		float menuItemHeight = 0.2f;
		float deltaY = 0.25f;
		float UV[8];

		// Create a shader program to use for the menu
		if (!getOpenGLRenderer()->createShaderProgram(
			&menuShaderProgramId,
			resourceFilenameVS.c_str(),
			resourceFilenameFS.c_str()))
		{
			return false;
		}

		// Set the generated shader program in the menu object
		menu->setShaderProgramId(menuShaderProgramId);

		TGAFILE tgaFile;
		tgaFile.imageData = NULL;

		if (LoadTGAFile(resourceFilenameTexture.c_str(), &tgaFile))
		{
			if (tgaFile.imageData == NULL ||
				tgaFile.imageHeight < 0 ||
				tgaFile.imageWidth < 0)
			{
				if (tgaFile.imageData != NULL)
				{
					delete[] tgaFile.imageData;
				}
				return false;
			}

			// Create a texture object for the menu, and copy the texture data to graphics memory
			if (!getOpenGLRenderer()->createTextureObject(
				&textureObjectId,
				tgaFile.imageData,
				tgaFile.imageWidth,
				tgaFile.imageHeight
			))
			{
				return false;
			}

			// Set the generated texture object in the menu object
			menu->setTextureObjectId(textureObjectId);

			// Texture data is stored in graphics memory now, we don't need this copy anymore
			if (tgaFile.imageData != NULL)
			{
				delete[] tgaFile.imageData;
			}
		}
		else
		{
			// Free texture data
			if (tgaFile.imageData != NULL)
			{
				delete[] tgaFile.imageData;
			}

			return false;
		}

		std::vector<std::string> menuOptions;
		menuOptions.push_back("Load World");
		menuOptions.push_back("Options");
		menuOptions.push_back("Exit");

		UV[0] = 0.01f;  // top left U
		UV[1] = 1.0f;   // top left V
		UV[2] = 0.99f;  // top right U
		UV[3] = 1.0f;   // top right V
		UV[4] = 0.01f;  // bottom left U
		UV[5] = 0.75f;  // bottom left V
		UV[6] = 0.99f;  // bottom right U
		UV[7] = 0.75f;  // bottom right V

		for (size_t i = 0; i < menuOptions.size(); i++)
		{
			// Allocate graphics memory for menu item
			generatedMenuItemGeometry = getOpenGLRenderer()->allocateGraphicsMemoryForMenuItem(
				currentX,
				currentY,
				menuItemHeight,
				UV,
				&menuShaderProgramId,
				&vaoMenuItemId,
				&colorUniformLocation,
				&textureUniformLocation);

			// If operation failed
			if (!generatedMenuItemGeometry)
			{
				menu->cleanupGraphicsObjects(getOpenGLRenderer());
				return false;
			}

			menu->setColorUniformLocation(colorUniformLocation);
			menu->setTextureUniformLocation(textureUniformLocation);

			menu->addMenuItem(menuOptions[i].c_str(), currentX, currentY, vaoMenuItemId);
			currentY -= deltaY;

			UV[1] -= 0.25f;
			UV[3] -= 0.25f;
			UV[5] -= 0.25f;
			UV[7] -= 0.25f;

			// Custom hack, texture used is the same for the exam, but the "exit" option is the 4th, not 3rd, so increment twice
			if (i == 1)
			{
				UV[1] -= 0.25f;
				UV[3] -= 0.25f;
				UV[5] -= 0.25f;
				UV[7] -= 0.25f;
			}
		}

		return true;
	}

	return false;
}

/* */
void CProyectoFinal::update(double deltaTime)
{
	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Save current delta time
	m_currentDeltaTime = deltaTime;
}

/* */
void CProyectoFinal::render()
{
	CGameMenu *menu = getMenu();
	CVector3 objPos2;
	objPos2.setValues(m_objectPosition.getX() + 2.5f, m_objectPosition.getY(), m_objectPosition.getZ());

	// If menu is active, render menu
	if (menu != NULL && menu->isInitialized() && menu->isActive())
	{
		menu->render(getOpenGLRenderer());
	}
	else // Otherwise, render active object if loaded (or test cube if no object is loaded)
	{
		if (m_pWorld != NULL)
		{
			m_pWorld->render(getCamera()->position);	
		}
		else
		{
			loadWorld();
		}
	}
}

/* */
bool CProyectoFinal::loadWorld()
{
	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;
	std::string resourceFilenameVS;
	std::string resourceFilenameFS;

	// If resource files cannot be found, return
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_3D_OBJECTS, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_3D_OBJECTS, wresourceFilenameFS, resourceFilenameFS))
	{
		Log << "ERROR: Unable to find one or more resources: " << endl;
		Log << "  " << VERTEX_SHADER_3D_OBJECTS << endl;
		Log << "  " << FRAGMENT_SHADER_3D_OBJECTS << endl;

		return false;
	}

	// Unload any current 3D model
	unloadWorld();

	// Create new World
	m_pWorld = new CWorld();

	// Initialize & Allocate graphics memory for object
	bool loaded = m_pWorld->initialize(getOpenGLRenderer());

	// If error ocurred, cleanup memory
	if (!loaded)
	{
		unloadWorld();
		return false;
	}

	return loaded;
}

/* */
void CProyectoFinal::unloadWorld()
{
	if (m_pWorld != NULL)
	{
		// Delete 3D object
		delete m_pWorld;
		m_pWorld = nullptr;
	}
}

/* */
void CProyectoFinal::onF2(int mods)
{
	std::wstring wideStringBuffer = L"buffer";
	wideStringBuffer.resize(MAX_PATH);

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"Obj Files\0*.obj\0" L"3D Studio Files\0*.3ds\0";
	ofn.lpstrFile = &wideStringBuffer[0];
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = L"Select a model file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wideStringBuffer[0], (int)wideStringBuffer.size(), NULL, 0, NULL, NULL);
		std::string multibyteString(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wideStringBuffer[0], (int)wideStringBuffer.size(), &multibyteString[0], size_needed, NULL, NULL);
		Log << "Filename to load: " << multibyteString.c_str() << endl;

		if (!loadWorld())
		{
			Log << "Unable to load 3D model" << endl;
		}
	}
}

/* */
void CProyectoFinal::onF3(int mods)
{
	// Check BITWISE AND to detect shift/alt/ctrl
	if (mods & KEY_MOD_SHIFT)
	{
		moveCamera(-1.0f);
	}
	else
	{
		moveCamera(1.0f);
	}
}

void CProyectoFinal::onF5(int mods)
{
	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	TCHAR appName[] = TEXT("Notepad");
	if (CreateProcess(NULL, appName, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
	{
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
}

void CProyectoFinal::onArrowUp(int mods)
{
	CVector3 movement = { 0,0,-DEFAULT_MOVEMENT_SPEED };
	m_objectPosition += movement;
}

void CProyectoFinal::onArrowDown(int mods)
{
	CVector3 movement = { 0,0,DEFAULT_MOVEMENT_SPEED };
	m_objectPosition += movement;
}

void CProyectoFinal::onArrowLeft(int mods)
{
	CVector3 movement = { -DEFAULT_MOVEMENT_SPEED,0,0 };
	m_objectPosition += movement;
}

void CProyectoFinal::onArrowRight(int mods)
{
	CVector3 movement = { DEFAULT_MOVEMENT_SPEED,0,0 };
	m_objectPosition += movement;
}

void CProyectoFinal::onMouseMove(float dx, float dy)
{
	CCamera* cam = getCamera();
	if (cam != nullptr) cam->Move(
		-dx * DEFAULT_CAMERA_MOVE_SPEED,
		dy * DEFAULT_CAMERA_MOVE_SPEED
	);
}

/* */
void CProyectoFinal::moveCamera(float direction)
{
	if (getOpenGLRenderer() != NULL)
	{
		getOpenGLRenderer()->moveCamera(direction);
	}
}

/* */
void CProyectoFinal::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		int option = getMenu()->getSelectedMenuItemNumber();

		switch (option)
		{
		case 1:
			onF2(0);
			break;
		case 2:
			// Not implemented
			Log << "<MENU OPTION NOT IMPLEMENTED>" << endl;
			break;
		case 3:
			if (getGameWindow() != NULL)
			{
				getGameWindow()->requestWindowClose();
			}
			break;
		}
	}
}