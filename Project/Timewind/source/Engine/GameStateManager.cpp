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
	// If in "normal" gameplay
	if (currentState <= GameStates::Running)
	{
		// If we get an input to pause
		if (_InputManager->CheckInputStatus(InputManager::Inputs::Pause) == InputManager::InputStatus::Pressed)
		{
			SetCurrentMenu(new Menu(Menu::MenuType::Pause));
		}
	}
	// If interacting with a menu
	else if (currentState == GameStates::Menu)
	{
		// Checks that we have a current menu
		if (currentMenu)
		{
			// Checks for a cancel input to close the pause or quit confirmation menus
			if (_InputManager->CheckInputStatus(InputManager::Inputs::MenuBack) == InputManager::InputStatus::Pressed)
			{
				TurnOffCurrentMenu();
			}
			// If the player didn't press to leave the menu, we can interact with the rest of the menu
			else
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
		Sets a new active game state

	\param newGameState
		The id of the new scene
*/
/*************************************************************************************************/
void GameStateManager::SetGameState(GameStates newGameState)
{
	// Sets the new state
	currentState = newGameState;

	// Clears the stacks
	while (!previousStates.empty())
	{
		previousStates.pop();
	}
	while (!previousMenus.empty())
	{
		previousMenus.pop();
	}
	currentMenu = NULL;
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
	// Error check
	if (newMenu)
	{
		// Adds the previous game state to the stack
		previousStates.push(currentState);
		if (currentState == GameStates::Menu)
		{
			previousMenus.push(currentMenu);
		}

		// Sets the new menu and puts us in the menu state
		currentMenu = newMenu;
		currentState = GameStates::Menu;
	}
}

/*************************************************************************************************/
/*!
	\brief
		Turns off the current menu and rolls back the game state
*/
/*************************************************************************************************/
void GameStateManager::TurnOffCurrentMenu()
{
	currentMenu->TurnOffMenu(true);
	currentState = previousStates.top();
	previousStates.pop();

	// If we are going back to a menu, gets that menu
	if (currentState == GameStates::Menu)
	{
		currentMenu = previousMenus.top();
		previousMenus.pop();
	}
	else
	{
		currentMenu = NULL;
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
