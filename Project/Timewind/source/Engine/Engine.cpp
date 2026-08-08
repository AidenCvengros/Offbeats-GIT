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
#include "AudioManager.h"
#include "GameStateManager.h"
#include "Debug.h"

// Additional includes
#include "../Game_Objects/Camera.h"
#include "../Scenes/Scene.h"
#include <sstream>

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
	// Creates the debug system. The init needs to happen first so we can start storing messages
	debugSystem = new Debug();
	debugSystem->Init();

	// Creates and initializes the game window
	gameWindow = new Window(1200, 900, "Retrofit");
	gameWindow->Init();

	// Adds essential game systems to the system list
	InputManager* inputManager = new InputManager();
	AudioManager* audioManager = new AudioManager();
	GameObjectManager* objectManager = new GameObjectManager();
	TextureManager* texManager = new TextureManager();
	SceneManager* sceneManager = new SceneManager();
	MapMatrix* mapMatrix = new MapMatrix(100, 50);
	EffectManager* effectManager = new EffectManager();
	GameStateManager* gameStateManager = new GameStateManager();

	// Creates and pushes back systems
	systemList[System::SystemTypes::inputManager] = inputManager;
	systemList[System::SystemTypes::audioManager] = audioManager;
	systemList[System::SystemTypes::mapMatrix] = mapMatrix;
	systemList[System::SystemTypes::gameObjectManager] = objectManager;
	systemList[System::SystemTypes::textureManager] = texManager;
	systemList[System::SystemTypes::sceneManager] = sceneManager;
	systemList[System::SystemTypes::effectManager] = effectManager;
	systemList[System::SystemTypes::gameStateManager] = gameStateManager;

	// Loops through, initializing each system
	for (const auto& [key, system] : systemList)
	{
		system->Init();
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
		// Updates the frame count
		frameCount++;

		// Announces the frame count
		std::stringstream frameCountString;
		frameCountString << "Start of frame " << frameCount;
		_Debug->Print(Debug::MessageType::Debug, frameCountString.str());

		// Tracks how long it has been since the previous frame
		double newTime = glfwGetTime();
		double dt = std::min(newTime - lastTime, 0.1);
		lastTime = newTime;
		dt = fmin(dt, 0.1);
		totalTime += dt;
#ifdef _DEBUG
		// Prints out the framerate. Debug only
		//std::cout << 1 / dt << std::endl;
#endif // _DEBUG

		// Updates the window and debug system
		_Window->Update(dt);
		_Debug->Update(dt);

		// Loops through, updating each system
		for (const auto& [key, system] : systemList)
		{
			system->Update(dt);
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
	_Debug->Draw();
	for (const auto& [key, system] : systemList)
	{
		system->Draw();
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
	_Debug->Print(Debug::MessageType::Debug, "Engine: Shutting down systems");

	// Loops through, shutting down each system
	for (const auto& [key, system] : systemList)
	{
		system->Shutdown();
	}

	// Closes the window
	_Window->Shutdown();

	// Closes the debug system
	_Debug->Shutdown();
}

/*********************************************************************************************/
/*!
	\brief
		Gets a system from the system list of the given type

	\param systemType
		The given system type
*/
/*********************************************************************************************/
System* Engine::GetSystem(System::SystemTypes systemType)
{
	// Checks for a window system
	if (systemType == System::SystemTypes::window)
	{
		return (System*)gameWindow;
	}
	// Checks for the debug system
	else if (systemType == System::SystemTypes::debugManager)
	{
		return (Debug*)debugSystem;
	}
	// Otherwise returns the system
	else
	{
		return systemList[systemType];
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
	return ((SceneManager*)GetSystem(System::SystemTypes::sceneManager))->GetCurrentScene();
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
Engine::Engine() : systemList(), gameWindow(NULL), debugSystem(NULL), lastTime(0.0), totalTime(0.0), frameCount(0)
{
	
}
