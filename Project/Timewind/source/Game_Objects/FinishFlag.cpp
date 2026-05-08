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
		// Tells the scene manager to switch scenes
		_SceneManager->ChangeScene(targetScene);
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
