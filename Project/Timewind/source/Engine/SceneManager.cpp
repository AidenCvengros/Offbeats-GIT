/*************************************************************************************************/
/*!
\file SceneManager.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.5.22
\brief
	Manages loading, unloading, and running scenes

    Functions include:
        + [FILL]

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "SceneManager.h"

// List of scenes
#include "../Scenes/Sandbox.h"

//-------------------------------------------------------------------------------------------------
// Private Constants
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Declarations
//-------------------------------------------------------------------------------------------------

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
		Constructor for the scene manager class

	\param engine
		The engine of the scene manager
*/
/*************************************************************************************************/
SceneManager::SceneManager()
{
	// Adds the list of scenes
	sceneList.push_back(new Sandbox());

	// Sets the scene indices
	currentSceneIndex = 0;
	nextSceneIndex = 0;
}

/*************************************************************************************************/
/*!
	\brief
		Initializes the scene manager (defaults to: sandbox)
*/
/*************************************************************************************************/
void SceneManager::Init()
{
	// Starts by loading the initial scene
	sceneList[currentSceneIndex]->LoadScene();
}

/*************************************************************************************************/
/*!
	\brief
		Updates the current scene

	\param
		The time elapsed since the previous frame.
*/
/*************************************************************************************************/
void SceneManager::Update(double dt)
{
	// Checks if the scene should change
	if (changeScene)
	{
		// Unloads the previous scene
		sceneList[currentSceneIndex]->UnloadScene();

		// Loads the next scene
		sceneList[nextSceneIndex]->LoadScene();

		// Corrects the current scene index
		currentSceneIndex = nextSceneIndex;

		// Sets that the scene no longer needs changing
		changeScene = false;
	}
}

/*************************************************************************************************/
/*!
	\brief
		Draws the current scene (currently empty)
*/
/*************************************************************************************************/
void SceneManager::Draw()
{

}

/*************************************************************************************************/
/*!
	\brief
		Shuts down the scene manager and all the scenes in it.
*/
/*************************************************************************************************/
void SceneManager::Shutdown()
{
	// Unloads the current scene
	sceneList[currentSceneIndex]->UnloadScene();

	// Walks through the scene list
	for (int i = 0; i < sceneList.size(); i++)
	{
		// Frees the scene
		delete sceneList[i];
	}

	// Clears the scene list
	sceneList.clear();
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
