/*************************************************************************************************/
/*!
\file GameStateManager.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.4.29
\brief
	Manages what menu the player is interacting with

    Functions include:
        + [FILL]

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "cppShortcuts.h"
#include "GameStateManager.h"

// Additional Includes
#include "../Gameplay/Menu.h"
#include "../Gameplay/MenuOptions/MenuOption.h"
#include "InputManager.h"
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
		Constructor for the menu manager class
*/
/*************************************************************************************************/
GameStateManager::GameStateManager() : System(SystemTypes::gameStateManager), currentState(GameStates::Walking), currentMenu(NULL)
{
	
}

/*************************************************************************************************/
/*!
	\brief
		Initializes the menu manager
*/
/*************************************************************************************************/
void GameStateManager::Init()
{
	
}

/*************************************************************************************************/
/*!
	\brief
		Updates the current menu

	\param
		The time elapsed since the previous frame.
*/
/*************************************************************************************************/
void GameStateManager::Update(double dt)
{
	// If interacting with a menu
	if (currentState == GameStates::Menu)
	{
		// Checks that we have a current menu
		if (currentMenu)
		{
			// Checks for menu inputs
			if (_InputManager->CheckInputStatus({ InputManager::Inputs::Down, InputManager::Inputs::MenuSelect }) == InputManager::InputStatus::Pressed)
			{
				currentMenu->IncrementOptionIndex();
			}
			else if (_InputManager->CheckInputStatus(InputManager::Inputs::Up) == InputManager::InputStatus::Pressed)
			{
				currentMenu->DecrementOptionIndex();
			}

			// Checks for a selection
			if (_InputManager->CheckInputStatus(InputManager::Inputs::MenuAdvance) == InputManager::InputStatus::Pressed)
			{
				currentMenu->GetSelectedOption()->Selected();
			}
			else
			{
				// Otherwise runs hovering behavior
				currentMenu->GetSelectedOption()->Hovering();
			}
		}
	}
}

/*************************************************************************************************/
/*!
	\brief
		Draws the current scene (currently empty)
*/
/*************************************************************************************************/
void GameStateManager::Draw()
{

}

/*************************************************************************************************/
/*!
	\brief
		Shuts down the scene manager and all the scenes in it.
*/
/*************************************************************************************************/
void GameStateManager::Shutdown()
{

}

/*************************************************************************************************/
/*!
	\brief
		Sets the current menu

	\param sceneID
		The id of the new scene
*/
/*************************************************************************************************/
void GameStateManager::SetCurrentMenu(Menu* newMenu)
{
	currentMenu = newMenu;
	currentState = GameStates::Menu;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
