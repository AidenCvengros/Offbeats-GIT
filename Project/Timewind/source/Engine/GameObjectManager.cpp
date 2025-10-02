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
#include "cppShortcuts.h"

// Includes the window class that game objects are getting drawn to
#include "Window.h"

// Includes the game object class cause this is the game object manager
#include "../Game_Objects/GameObject.h"

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
		// Destroys the game object if it is marked for destruction
		if ((*it).second->GetToBeDestroyed())
		{
			delete (*it).second;
			it = gameObjectList.erase(it);
		}
		// If the game object is active, update it
		else if ((*it).second->GetActive())
		{
			(*it).second->Update(dt);
			it++;
		}
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
		if (it->first < 100)
		{
			if ((*it).second->GetActive())
			{
				_Window->DrawGameObject((*it).second);
				(*it).second->DrawChildObjects();
			}
		}

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
		Draws the glitched game objects in the manager to the screen.
*/
/*************************************************************************************************/
void GameObjectManager::DrawGlitches()
{
	// Walks through the game object list
	for (std::multimap<int, GameObject*>::iterator it = gameObjectList.begin(); it != gameObjectList.end();)
	{
		if (it->first >= 100)
		{
			if ((*it).second->GetActive())
			{
				_Window->DrawGameObject((*it).second);
				(*it).second->DrawChildObjects();
			}
		}

		it++;
	}
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
