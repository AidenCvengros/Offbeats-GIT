/*************************************************************************************************/
/*!
\file Engine.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2023.9.7
\brief
    Creates and manages the game engine

    Public Functions:
        + Engine::createEngine
		+ Engine::~Engine
		+ Engine::Init
		+ Engine::Update
		+ Engine::Draw

	Private Functions:
		+ Engine::Engine

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Engine.h"
#include "cppShortcuts.h"

// Includes glfw to track if window is open
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Properly includes the window class
#include "Window.h"

// Headers for derived system classes to be added to the system list
#include "GameObjectManager.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "../Gameplay/MapMatrix.h"
#include "EffectManager.h"

// Adds the camera class so a starting camera can be set
#include "../Game_Objects/Camera.h"

// Adds the scene class so we can find the current scene from the engine
#include "../Scenes/Scene.h"

//-------------------------------------------------------------------------------------------------
// Private Constants
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Declarations
//-------------------------------------------------------------------------------------------------

Engine* Engine::engineInstance;			// Forward reference for the engine instance

Texture* tempTexture;																				// TEMP CODE
Texture* tempTexture2;																				// TEMP CODE

//-------------------------------------------------------------------------------------------------
// Private Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Function Declarations
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Private Function Declarations
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Static function that creates the singleton game engine. Also will return the game engine
		reference
		
	\return
		Returns the game engine.
*/
/*************************************************************************************************/
Engine* Engine::createEngine()
{
	// Checks to prevent multiple engine instances
	if (!engineInstance)
	{
		// Creates the engine
		engineInstance = new Engine();
		engineInstance->Init();
	}

	// Returns the game engine
	return engineInstance;
}

/*********************************************************************************************/
/*!
	\brief
		Engine destructor.
*/
/*********************************************************************************************/
Engine::~Engine()
{

}

/*********************************************************************************************/
/*!
	\brief
		Initializes the engine
*/
/*********************************************************************************************/
void Engine::Init()
{
	// Creates and initializes the game window
	gameWindow = new Window(1200, 900, "Retrofit");
	gameWindow->Init();

	// Adds essential game systems to the system list
	InputManager* inputManager = new InputManager();
	GameObjectManager* objectManager = new GameObjectManager();
	TextureManager* texManager = new TextureManager();
	SceneManager* sceneManager = new SceneManager();
	MapMatrix* mapMatrix = new MapMatrix(100, 50);
	EffectManager* effectManager = new EffectManager();

	// Creates and pushes back systems
	systemList.push_back((System*)inputManager);
	systemList.push_back((System*)objectManager);
	systemList.push_back((System*)texManager);
	systemList.push_back((System*)sceneManager);
	systemList.push_back((System*)mapMatrix);
	systemList.push_back((System*)effectManager);

	// Loops through, initializing each system
	for (size_t i = 0; i < systemList.size(); i++)
	{
		systemList[i]->Init();
	}
}

/*********************************************************************************************/
/*!
	\brief
		Updates the game engine. Also defines an in-engine frame
*/
/*********************************************************************************************/
void Engine::Update()
{
	// Creates the game loop until the window is closed
	while (!glfwWindowShouldClose(_Window->GetVulkanWindowPtr()))
	{
		// Tracks how long it has been since the previous frame
		double newTime = glfwGetTime();
		double dt = newTime - lastTime;
		lastTime = newTime;
		fmin(dt, 0.1);
#ifdef _DEBUG
		// Prints out the framerate. Debug only
		//std::cout << 1 / dt << std::endl;
#endif // _DEBUG

		// Updates the window
		_Window->Update(dt);

		// Loops through, updating each system
		for (size_t i = 0; i < systemList.size(); i++)
		{
			systemList[i]->Update(dt);
		}

		// After updating, draws to screen
		Draw();
	}
}

/*********************************************************************************************/
/*!
	\brief
		Draws the engine updates to the screen.
*/
/*********************************************************************************************/
void Engine::Draw()
{
	// Sets up the window to draw
	_Window->Draw();

	// Loops through, drawing each system
	for (size_t i = 0; i < systemList.size(); i++)
	{
		systemList[i]->Draw();
	}

	// Starts the mask render pass draw
	_Window->DrawMaskRenderPass();
	_GameObjectManager->DrawGlitches();

	// Finishes the drawing commands for this frame
	_Window->CleanupDraw();
}

/*********************************************************************************************/
/*!
	\brief
		Shuts down the game engine
*/
/*********************************************************************************************/
void Engine::Shutdown()
{
	// Loops through, shutting down each system
	for (size_t i = 0; i < systemList.size(); i++)
	{
		systemList[i]->Shutdown();
	}

	// Closes the window
	_Window->Shutdown();
}

/*********************************************************************************************/
/*!
	\brief
		Gets a system from the system list of the given type

	\param systemType
		The given system type
*/
/*********************************************************************************************/
System* Engine::GetSystem(SystemTypes systemType)
{
	// Checks for a window system
	if (systemType == SystemTypes::window)
	{
		return (System*)gameWindow;
	}
	// Checks for an input manager
	else if (systemType == SystemTypes::inputManager)
	{
		for (int i = 0; i < systemList.size(); i++)
		{
			if (dynamic_cast<InputManager*>(systemList[i]) != NULL)
			{
				return systemList[i];
			}
		}
		return systemList[0];
	}
	// Checks for a scene manager
	else if (systemType == SystemTypes::sceneManager)
	{
		for (int i = 0; i < systemList.size(); i++)
		{
			if (dynamic_cast<SceneManager*>(systemList[i]) != NULL)
			{
				return systemList[i];
			}
		}
	}
	// Checks for a game object manager
	else if (systemType == SystemTypes::gameObjectManager)
	{
		for (int i = 0; i < systemList.size(); i++)
		{
			if (dynamic_cast<GameObjectManager*>(systemList[i]) != NULL)
			{
				return systemList[i];
			}
		}
	}
	// Checks for a texture manager
	else if (systemType == SystemTypes::textureManager)
	{
		for (int i = 0; i < systemList.size(); i++)
		{
			if (dynamic_cast<TextureManager*>(systemList[i]) != NULL)
			{
				return systemList[i];
			}
		}
	}
	// Checks for a map matrix
	else if (systemType == SystemTypes::mapMatrix)
	{
		for (int i = 0; i < systemList.size(); i++)
		{
			if (dynamic_cast<MapMatrix*>(systemList[i]) != NULL)
			{
				return systemList[i];
			}
		}
	}
	// Checks for an effect manager
	else if (systemType == SystemTypes::effectManager)
	{
		for (int i = 0; i < systemList.size(); i++)
		{
			if (dynamic_cast<EffectManager*>(systemList[i]) != NULL)
			{
				return systemList[i];
			}
		}
	}

	// Otherwise return null
	return NULL;
}

/*********************************************************************************************/
/*!
	\brief
		Gets the currently running scene
*/
/*********************************************************************************************/
Scene* Engine::GetCurrentScene()
{
	return ((SceneManager*)GetSystem(Engine::SystemTypes::sceneManager))->GetCurrentScene();
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*********************************************************************************************/
/*!
	\brief
		Engine class initializer.
*/
/*********************************************************************************************/
Engine::Engine()
{
	
}
