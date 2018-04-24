#include "../stdafx.h"

#include "windows.h"

#include <vector>
#include <string>

#include "../Include/Globals.h"
#include "../Include/CProyectoFinal.h"
#include "../Include/C3DModel.h"
#include "../Include/LoadTGA.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/LineTracer.h"
#include "../UTFConvert.h"

/* */
CProyectoFinal::CProyectoFinal() :
	m_pWorld(NULL),
	m_currentDeltaTime{ 0.0 }
{
	Log << "Constructor: CProyectoFinal()" << endl;
}

/* */
CProyectoFinal::CProyectoFinal(int window_width, int window_height) :
	CApp(window_width, window_height),
	m_pWorld(NULL),
	m_currentDeltaTime{ 0.0 }
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

			if (!initializeMCCube())
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

bool CProyectoFinal::initializeMCCube()
{
	std::wstring wresourceFilenameTexture;
	std::string resourceFilenameTexture;

	// If resource files cannot be found, return
	if (!CWideStringHelper::GetResourceFullPath(MC_CUBE_TEXTURE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << MC_CUBE_TEXTURE << endl;
		return false;
	}

	// Initialize the texture
	unsigned int mcCUbeTextureID = -1;

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
			&mcCUbeTextureID,
			tgaFile.imageData,
			tgaFile.imageWidth,
			tgaFile.imageHeight
		))
		{
			return false;
		}

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

	// Initialize a Minecraft cube
	getOpenGLRenderer()->initializeMCCube(mcCUbeTextureID);

	return true;
}

/* */
void CProyectoFinal::update(double deltaTime)
{
	double degreesToRotate = 0.0;

	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Save current delta time
	m_currentDeltaTime = deltaTime;

	// Calculate degrees to rotate
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// degrees = rotation speed * delta time 
	// deltaTime is expressed in milliseconds, but our rotation speed is expressed in seconds (convert delta time from milliseconds to seconds)
	degreesToRotate = DEFAULT_ROTATION_SPEED * (deltaTime / 1000.0);
	// accumulate rotation degrees
	m_objectRotation += degreesToRotate;

	// Reset rotation if needed
	while (m_objectRotation > 360.0)
	{
		m_objectRotation -= 360.0;
	}
	if (m_objectRotation < 0.0)
	{
		m_objectRotation = 0.0;
	}
}

/* */
void CProyectoFinal::render()
{
	CGameMenu *menu = getMenu();

	// If menu is active, render menu
	if (menu != NULL && menu->isInitialized() && menu->isActive())
	{
		menu->render(getOpenGLRenderer());
	}
	else // Otherwise, render active object if loaded (or test cube if no object is loaded)
	{
		if (m_pWorld != NULL && m_pWorld->isInitialized)
		{
			auto cam = getCamera();
			m_pWorld->render(cam->getPosition(), cam->getRotation());
		}
		else if(!worldLoading)
		{
			worldLoading = true;
			loadWorldHandler = CreateThread(NULL, 0, staticLoadWorld, this, 0, &loadWorldThreadID);
		}
		else
		{
			// convert total degrees rotated to radians;
			double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;

			// Get a matrix that has both the object rotation and translation
			MathHelper::Matrix4 modelMatrix = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, CVector3(0, 0, 0));

			getOpenGLRenderer()->renderMCCube(&modelMatrix);
		}
	}
}

DWORD CProyectoFinal::staticLoadWorld(PVOID param)
{
	CProyectoFinal *myApp = (CProyectoFinal*)param;
	Log << "Thread 1: Inicializando Grid" << endl;
	bool loaded = myApp->loadWorld();
	Log << "Thread 1: Grid Inicializada" << endl;
	return loaded ? 0 : -1;
}

/* */
bool CProyectoFinal::loadWorld()
{
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

	worldLoading = false;
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
	CCamera* cam = getCamera();
	if (cam != nullptr) cam->Move(
		0, 0, -DEFAULT_MOVEMENT_SPEED);
}

void CProyectoFinal::onArrowDown(int mods)
{
	CCamera* cam = getCamera();
	if (cam != nullptr) cam->Move(
		0, 0, DEFAULT_MOVEMENT_SPEED);
}

void CProyectoFinal::onArrowLeft(int mods)
{
	CCamera* cam = getCamera();
	if (cam != nullptr) cam->Move(
		-DEFAULT_MOVEMENT_SPEED, 0, 0);
}

void CProyectoFinal::onArrowRight(int mods)
{
	CCamera* cam = getCamera();
	if (cam != nullptr) cam->Move(
		DEFAULT_MOVEMENT_SPEED, 0, 0);
}

void CProyectoFinal::onMouseMove(float dx, float dy)
{
	CCamera* cam = getCamera();
	if (cam != nullptr) cam->Rotate(
		-dx * DEFAULT_ROTATION_SPEED,
		dy * DEFAULT_ROTATION_SPEED
	);
}

void CProyectoFinal::onSpaceBar(int mods)
{
	CCamera* cam = getCamera();
	if (cam != nullptr) cam->Move(
		0, DEFAULT_MOVEMENT_SPEED, 0);
}

void CProyectoFinal::onCKey(int mods)
{
	CCamera* cam = getCamera();
	if (cam != nullptr) cam->Move(
		0, -DEFAULT_MOVEMENT_SPEED, 0);
}

void CProyectoFinal::onMouseLeftClick()
{
	std::vector<CVector3> line;

	LineTracer::LineSamples(
		getCamera()->getPosition(), getCamera()->getLookAt(), &line);

	for (size_t i = 0; i < line.size(); i++)
	{
		std::cout << "LineTrace: "
			<< line[i].getX() << ", "
			<< line[i].getY() << ", "
			<< line[i].getZ() << std::endl;
	}
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