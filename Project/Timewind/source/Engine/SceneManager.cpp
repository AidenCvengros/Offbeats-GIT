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
#include "../Scenes/Level1.h"
#include "../Scenes/Level1_2.h"
#include "../Scenes/Level1_3.h"
#include "../Scenes/Level1_4.h"

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
	sceneList.push_back(new Level1());
	sceneList.push_back(new Level1_2());
	sceneList.push_back(new Level1_3());
	sceneList.push_back(new Level1_4());
	sceneList.push_back(new Sandbox());

	// Sets the scene indices
	currentSceneIndex = 4;
	nextSceneIndex = currentSceneIndex;
	changeScene = false;
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

/*************************************************************************************************/
/*!
	\brief
		Sets this scene to be changed to

	\param sceneID
		The id of the new scene
*/
/*************************************************************************************************/
void SceneManager::ChangeScene(int sceneID)
{
	// Checks that the scene is valid
	int newSceneIndex = FindSceneIndex(sceneID);
	if (newSceneIndex >= 0)
	{
		// Sets as the new scene
		nextSceneIndex = newSceneIndex;
		changeScene = true;
	}
}

/*************************************************************************************************/
/*!
	\brief
		Finds the first listed scene with the given id

	\param sceneID
		The id of the scene

	\return
		The requested scene. Returns NULL if no matching id is found
*/
/*************************************************************************************************/
Scene* SceneManager::FindScene(int sceneID)
{
	// Walks through the scene list
	for (int i = 0; i < sceneList.size(); i++)
	{
		// Checks if the scene id matches
		if (sceneList[i]->GetSceneID() == sceneID)
		{
			return sceneList[i];
		}
	}

	// Otherwise returns NULL
	return NULL;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Finds the first listed scene index with the given id

	\param sceneID
		The id of the scene

	\return
		The requested scene index. Returns -1 if no matching id was found.
*/
/*************************************************************************************************/
int SceneManager::FindSceneIndex(int sceneID)
{
	// Walks through the scene list
	for (int i = 0; i < sceneList.size(); i++)
	{
		// Checks if the scene id matches
		if (sceneList[i]->GetSceneID() == sceneID)
		{
			return i;
		}
	}

	// Otherwise returns NULL
	return -1;
}
