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
#include "Window.h"
#include "AudioManager.h"
#include "../Visuals/CameraMovement.h"
#include "../Game_Objects/Camera.h"
#include "SceneManager.h"
#include "GameObjectManager.h"
#include "../Gameplay/Inventory.h"

// Includes glfw input reading functionality
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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
GameStateManager::GameStateManager() : System(SystemTypes::gameStateManager), currentState(GameStates::Walking), currentMenu(NULL), paused(false)
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
	glfwSetInputMode(_Window->GetVulkanWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
			_AudioManager->PauseAudio();
			paused = true;
		}
		// Otherwise, if we are placing we can update the inventory menu
		else if (currentState == GameStates::Placing)
		{
			UpdateCurrentMenu();
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
				UpdateCurrentMenu();
			}
		}
	}
	// If in a cutscene
	else if (currentState == GameStates::Cutscene)
	{
		// Updates the cutscene
		UpdateCutscene(dt);
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
		previousMenus.top()->TurnOffMenu();
		previousMenus.pop();
	}
	if (currentMenu)
	{
		currentMenu->TurnOffMenu();
	}
	currentMenu = NULL;

	// If we were paused, this will leave paused
	if (paused)
	{
		paused = false;
		_AudioManager->RestartAudio();
	}

	glfwSetInputMode(_Window->GetVulkanWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/*************************************************************************************************/
/*!
	\brief
		Sets the current menu

	\param sceneID
		The id of the new scene
	
	\param isPlacing
		Set true if game mode should be set to placing
*/
/*************************************************************************************************/
void GameStateManager::SetCurrentMenu(Menu* newMenu, bool isPlacing)
{
	// Error check
	if (newMenu)
	{
		// Adds the previous game state to the stack
		previousStates.push(currentState);
		if (currentState == GameStates::Menu || currentState == GameStates::Placing)
		{
			previousMenus.push(currentMenu);
		}

		// Sets the new menu and puts us in the menu state
		currentMenu = newMenu;
		if (!isPlacing)
		{
			currentState = GameStates::Menu;
			glfwSetInputMode(_Window->GetVulkanWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			currentState = GameStates::Placing;
			glfwSetInputMode(_Window->GetVulkanWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
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
	if (currentState == GameStates::Menu || currentState == GameStates::Placing)
	{
		currentMenu = previousMenus.top();
		previousMenus.pop();
	}
	else
	{
		currentMenu = NULL;
		glfwSetInputMode(_Window->GetVulkanWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Checks if we were paused
		if (paused)
		{
			paused = false;
			_AudioManager->RestartAudio();
		}
	}
}

/*************************************************************************************************/
/*!
	\brief
		Sets the current menu

	\param newGameState
		The game state that will be after the scene refreshes
*/
/*************************************************************************************************/
void GameStateManager::RefreshCurrentScene(GameStates newGameState)
{
	// Sets the game state to a cutscene
	SetGameState(GameStates::Cutscene);

	// Adds in transition actions
	glm::vec3 relativeVec = glm::normalize(_Window->GetCamera()->GetLookAtVector());
	float horizAngle = atanf(relativeVec.z / relativeVec.x);
	if (relativeVec.x >= 0.0f)
	{
		horizAngle += glm::pi<float>();
	}
	horizAngle = glm::pi<float>() - horizAngle;
	cutsceneScript.push(std::make_pair(CutsceneActions::CameraMovement, new CameraMovement(CameraMovement::MovementType::Revolution, 0.5, _Window->GetCamera()->Get3DPosition(), { _Window->GetCamera()->GetLookAtPosition(), 0.0f }, { -horizAngle, 0.0f, _Window->GetCamera()->Get3DPosition().z }, _Window->GetCamera()->GetPerspective(), 1.0f - _Window->GetCamera()->GetPerspective())));
	cutsceneScript.push(std::make_pair(CutsceneActions::RefreshScene, (void*)NULL));
	cutsceneScript.push(std::make_pair(CutsceneActions::CameraMovement, new CameraMovement(CameraMovement::MovementType::Revolution, 0.5, { _Window->GetCamera()->GetLookAtPosition().x - _Window->GetCamera()->Get3DPosition().z, _Window->GetCamera()->GetLookAtPosition().y, 0.0f }, { _Window->GetCamera()->GetLookAtPosition(), 0.0f }, { -glm::pi<float>() / 2.0f, 0.0f, _Window->GetCamera()->Get3DPosition().z }, 1.0f - _Window->GetCamera()->GetPerspective(), 1.0f - _Window->GetCamera()->GetPerspective())));
	cutsceneScript.push(std::make_pair(CutsceneActions::GameStateChange, new GameStates(newGameState)));
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Checks for inputs to affect the current menu
*/
/*************************************************************************************************/
void GameStateManager::UpdateCurrentMenu()
{
	if (currentMenu->GetIsVertical())
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
	}
	else
	{
		// Checks for menu inputs
		if (_InputManager->CheckInputStatus({ InputManager::Inputs::Right, InputManager::Inputs::MenuSelect }) == InputManager::InputStatus::Pressed)
		{
			currentMenu->IncrementOptionIndex();
		}
		else if (_InputManager->CheckInputStatus(InputManager::Inputs::Left) == InputManager::InputStatus::Pressed)
		{
			currentMenu->DecrementOptionIndex();
		}
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

/*************************************************************************************************/
/*!
	\brief
		Checks for actions in the cutscene queue

	\param dt
		The time elapsed since the previous frame.
*/
/*************************************************************************************************/
void GameStateManager::UpdateCutscene(double dt)
{
	// Loops until either the queue is empty or we are waiting
	while (cutsceneScript.size())
	{
		// Checks if we have a camera movement
		if (cutsceneScript.front().first == CutsceneActions::CameraMovement)
		{
			// Gets the camera movement pointer
			CameraMovement* currentCameraMovement = (CameraMovement*)cutsceneScript.front().second;

			// Checks that the camera movement is still running
			if (currentCameraMovement->GetRemainingTime() > 0.0)
			{
				_Window->GetCamera()->StartCameraMovement(currentCameraMovement);

				// Now we wait for the camera movement
				break;
			}
			// Otherwise we clear it out
			else
			{
				delete currentCameraMovement;
				cutsceneScript.pop();
			}
		}
		// Checks if we have a wait command
		else if (cutsceneScript.front().first == CutsceneActions::Wait)
		{
			// Gets the wait time
			double* currentWait = (double*)cutsceneScript.front().second;

			// Decrements the time
			*currentWait -= dt;

			// If the time has run out, clears it
			if (*currentWait <= 0.0)
			{
				delete currentWait;
				cutsceneScript.pop();
			}
			// Otherwise waits
			else
			{
				break;
			}
		}
		// Checks if we have a game state change command
		else if (cutsceneScript.front().first == CutsceneActions::GameStateChange)
		{
			// Gets the new game state
			GameStates* newGameState = (GameStates*)cutsceneScript.front().second;

			// Sets the new game state
			if (*newGameState != GameStates::Menu)
			{
				SetGameState(*newGameState);
			}

			// Clears the command
			delete newGameState;
			cutsceneScript.pop();
		}
		// Checks if we have a scene change command
		else if (cutsceneScript.front().first == CutsceneActions::SceneChange)
		{
			// Gets the new scene id
			int* newSceneID = (int*)cutsceneScript.front().second;

			// Sets the new Scene
			_SceneManager->ChangeScene(*newSceneID);

			// Clears the command
			delete newSceneID;
			cutsceneScript.pop();
		}
		// Checks if we have a refresh scene command
		else if (cutsceneScript.front().first == CutsceneActions::RefreshScene)
		{
			// Goes through refresh protocol
			_GameObjectManager->GetPlayer()->GetInventory()->PlacingMode(false);
			_SceneManager->GetCurrentScene()->RefreshScene();
			_Window->GetCamera()->ResetCameraOffset();

			// Clears the command
			cutsceneScript.pop();
		}
		// Otherwise breaks the loop
		else
		{
			cutsceneScript.pop();
			break;
		}
	}
}
