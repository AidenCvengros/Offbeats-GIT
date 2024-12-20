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

#include "Engine.h"

// Includes glfw to track if window is open
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Headers for derived system classes to be added to the system list
#include "GameObjectManager.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "MapMatrix.h"
#include "EffectManager.h"

// Adds the camera class so a starting camera can be set
#include "Camera.h"

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
	gameWindow = new Window(800, 600, "Syncopatience");
	gameWindow->Init();

	// Adds essential game systems to the system list
	InputManager* inputManager = new InputManager(gameWindow);
	GameObjectManager* objectManager = new GameObjectManager(inputManager);
	TextureManager* texManager = new TextureManager(gameWindow);
	SceneManager* sceneManager = new SceneManager(this);
	MapMatrix* mapMatrix = new MapMatrix(25, 15);
	EffectManager* effectManager = new EffectManager();

	// Creates and pushes back systems
	systemList.push_back((System*)inputManager);
	systemList.push_back((System*)objectManager);
	systemList.push_back((System*)texManager);
	systemList.push_back((System*)sceneManager);
	systemList.push_back((System*)mapMatrix);
	systemList.push_back((System*)effectManager);

	// Sets up system references to each other
	sceneManager->SetMapMatrix(mapMatrix);
	sceneManager->SetEffectManager(effectManager);

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
	while (!glfwWindowShouldClose(gameWindow->GetWindowPtr()))
	{
		// Tracks how long it has been since the previous frame
		double newTime = glfwGetTime();
		dt = newTime - lastTime;
		lastTime = newTime;
#ifdef _DEBUG
		// Prints out the framerate. Debug only
		//std::cout << 1 / dt << std::endl;
#endif // _DEBUG

		// Updates the window
		gameWindow->Update(dt);

		// Loops through, updating each system
		for (size_t i = 0; i < systemList.size(); i++)
		{
			systemList[i]->Update(dt);
		}

		// After updating, draws to screen
		Draw(gameWindow);
	}
}

/*********************************************************************************************/
/*!
	\brief
		Draws the engine updates to the screen.

	\param window_
		The window to draw the engine to
*/
/*********************************************************************************************/
void Engine::Draw(Window* window_)
{
	// Sets up the window to draw
	gameWindow->Draw(window_);

	// Loops through, drawing each system
	for (size_t i = 0; i < systemList.size(); i++)
	{
		systemList[i]->Draw(window_);
	}

	// Finishes the drawing commands for this frame
	gameWindow->CleanupDraw();
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
	gameWindow->Shutdown();
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
