/*************************************************************************************************/
/*!
\file FinishFlag.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.3.19
\brief
    The finish flag object

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "FinishFlag.h"
#include "../Engine/cppShortcuts.h"

// Includes the scene manager
#include "../Engine/SceneManager.h"
#include "../Engine/TextureManager.h"
#include "../Engine/GameObjectManager.h"
#include "Text.h"
#include "Player.h"
#include "../Gameplay/MenuOptions/GoToSceneOption.h"
#include "../Gameplay/MenuOptions/RefreshSceneOption.h"

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

/*************************************************************************************************/
/*!
	\brief
		Updates the game object

	\param dt
		The time elapsed since the previous frame
*/
/*************************************************************************************************/
void FinishFlag::Update(double dt)
{
	// Checks if this flag is timed
	if (timed)
	{
		timer -= dt;

		// Checks if the timer has run out
		if (timer <= 0.0f)
		{
			// Stops running the timer
			timed = false;
			flagActive = false;

			// Makes like a dark, fucked up version of the finish flag haha. Just a glimpse into my dark reality. A full stare into my twisted perspective would make most simply go insane lmao
			SetDrawPriority(150);
			SetColor({ 0.9f, 0.0f, 0.5f, 0.7f });
			SetRotation(180.0f);
		}
	}
}

/*************************************************************************************************/
/*!
	\brief
		Activates the flag to go to the next scene
*/
/*************************************************************************************************/
void FinishFlag::JumpToTargetScene()
{
	// Checks that the flag is active
	if (flagActive)
	{
		// Creates the end of level menu
		Menu* endOfLevelMenu = new Menu(Menu::MenuType::EndOfLevel);
		Text* nextLevelText = new Text("Next Level", _TextureManager->GetDefaultFont(), 18, { -10.0f, 0.0f }, 0.0f, { 0.1f, 0.1f }, 91, { 1.0f, 1.0f, 1.0f, 1.0f });
		Text* retryLevelText = new Text("Retry Level", _TextureManager->GetDefaultFont(), 18, { -10.0f, -2.0f }, 0.0f, { 0.1f, 0.1f }, 91, { 1.0f, 1.0f, 1.0f, 1.0f });
		Text* mainMenuText = new Text("Main Menu", _TextureManager->GetDefaultFont(), 18, { -10.0f, -4.0f }, 0.0f, { 0.1f, 0.1f }, 91, { 1.0f, 1.0f, 1.0f, 1.0f });
		GameObject* background = new GameObject({ 0.0f, 0.0f }, 0.0f, { 24.0f, 12.0f }, 90, true, { 0.0f, 0.0f, 0.0f, 0.5f });
		nextLevelText->SetFollowingCamera(true);
		retryLevelText->SetFollowingCamera(true);
		mainMenuText->SetFollowingCamera(true);
		background->SetFollowingCamera(true);
		_GameObjectManager->AddGameObject(nextLevelText);
		_GameObjectManager->AddGameObject(retryLevelText);
		_GameObjectManager->AddGameObject(mainMenuText);
		GoToSceneOption* nextLevelOption = new GoToSceneOption(nextLevelText, targetScene);
		RefreshSceneOption* retryLevelOption = new RefreshSceneOption(retryLevelText, GameStateManager::GameStates::Walking);
		GoToSceneOption* mainMenuOption = new GoToSceneOption(mainMenuText, 0);
		endOfLevelMenu->AddOption(nextLevelOption);
		endOfLevelMenu->AddOption(retryLevelOption);
		endOfLevelMenu->AddOption(mainMenuOption);
		endOfLevelMenu->SetIsVertical(true);
		endOfLevelMenu->SetIsFragile(true);
		GameObject* menuCursor = new GameObject({ 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, 91, true, { 1.0f, 1.0f, 1.0f, 1.0f });
		_GameObjectManager->AddGameObject(menuCursor);
		endOfLevelMenu->SetCursorObject(menuCursor, { -1.0f, 1.0f });
		menuCursor->SetFollowingCamera(true);

		// Calculates the player's speed rank
		float horizontalVelocityRatio = _GameObjectManager->GetPlayer()->GetVelocity().x / _GameObjectManager->GetPlayer()->GetMaxSpeed();
		Text* speedRankText;
		if (horizontalVelocityRatio <= 0.501f)
		{
			speedRankText = new Text("C", _TextureManager->GetDefaultFont(), 24, { -1.0f, 3.0f }, 0.0f, { 0.1f, 0.1f }, 92, { 1.0f, 1.0f, 1.0f, 1.0f });
		}
		else if (horizontalVelocityRatio <= 0.8f)
		{
			speedRankText = new Text("B", _TextureManager->GetDefaultFont(), 28, { -1.2f, 3.0f }, 0.0f, { 0.1f, 0.1f }, 92, { 0.8f, 0.54f, 0.27f, 1.0f });
		}
		else if (horizontalVelocityRatio <= 0.99f)
		{
			speedRankText = new Text("A", _TextureManager->GetDefaultFont(), 32, { -1.4f, 3.0f }, 0.0f, { 0.1f, 0.1f }, 92, { 0.2f, 0.2f, 1.0f, 1.0f });
		}
		else
		{
			speedRankText = new Text("S", _TextureManager->GetDefaultFont(), 48, { -2.0f, 3.0f }, 20.0f, { 0.1f, 0.1f }, 92, { 0.93f, 0.75f, 0.02f, 1.0f });
		}
		_GameObjectManager->AddGameObject(speedRankText);
		speedRankText->SetFollowingCamera(true);
		endOfLevelMenu->AddMenuObject(speedRankText);

		// Sets the new menu as active
		_GameStateManager->SetCurrentMenu(endOfLevelMenu);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Starts the timer for the flag to glitch out

	\param timerLength
		How long the timer runs
*/
/*************************************************************************************************/
void FinishFlag::SetTimer(double timerLength)
{
	// Sets the timer and starts it
	timer = timerLength;
	timed = true;
}

//-------------------------------------------------------------------------------------------------
// Private Function Declarations
//-------------------------------------------------------------------------------------------------
