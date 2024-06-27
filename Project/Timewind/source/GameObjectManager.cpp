/*************************************************************************************************/
/*!
\file GameObjectManager.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.2.10
\brief
    Manager to keep track of all game objects in a scene and ensure they get drawn, cleared, loaded, etc.

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "GameObjectManager.h"
#include "Player.h"

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
		Initializes the game object manager.
*/
/*************************************************************************************************/
void GameObjectManager::Init()
{
	// Test game objects																				TO BE REMOVED ONCE GAME OBJECTS ARE SYSTEMATIZED
	//gameObjectList.push_back(new GameObject({ -0.5f, 1.0f }, -20.0f, { 1.0f, 1.0f }));
}

/*************************************************************************************************/
/*!
	\brief
		Updates the game objects

	\param
		The time elapsed since the previous frame.
*/
/*************************************************************************************************/
void GameObjectManager::Update(double dt)
{
	// Walks through the game object list
	for (std::multimap<int, GameObject*>::iterator it = gameObjectList.begin(); it != gameObjectList.end();)
	{
		(*it).second->Update(dt, inputManager);

		it++;
	}
}

/*************************************************************************************************/
/*!
	\brief
		Draws the game objects in the manager to the screen.
*/
/*************************************************************************************************/
void GameObjectManager::Draw()
{
	// Walks through the game object list
	for (std::multimap<int, GameObject*>::iterator it = gameObjectList.begin(); it != gameObjectList.end();)
	{
		window->DrawGameObject(*(*it).second);

		it++;
	}
}

/*************************************************************************************************/
/*!
	\brief
		Shuts down the game object manager and all game objects in it.
*/
/*************************************************************************************************/
void GameObjectManager::Shutdown()
{
	// Walks through the game object list
	for (std::multimap<int, GameObject*>::iterator it = gameObjectList.begin(); it != gameObjectList.end();)
	{
		// Frees the game object
		delete (*it).second;
		it++;
	}

	// Clears the game object list
	gameObjectList.clear();
}

/*************************************************************************************************/
/*!
	\brief
		Adds a game object to the manager's list.
*/
/*************************************************************************************************/
void GameObjectManager::AddGameObject(GameObject* newGameObject)
{
	gameObjectList.insert(std::pair(newGameObject->GetDrawPriority(), newGameObject));
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
