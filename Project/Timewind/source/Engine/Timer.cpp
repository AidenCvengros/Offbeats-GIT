/*************************************************************************************************/
/*!
\file Timer.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2025.9.1
\brief
    Class to create timers

    Functions include:
        + FILL

Copyright (c) 2025 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Timer.h"
#include "../Engine/cppShortcuts.h"

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
// Public Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Constructor for the Timer class

	\param _length
		The length of the timer

	\param _canBePaused
		Whether the timer pauses when the player is paused
*/
/*************************************************************************************************/
Timer::Timer(double _length, bool _canBePaused) : length(_length), canBePaused(_canBePaused)
{
	// Checks if we are using the true time or unpaused time
	if (canBePaused)
	{
		// Fetches the starting time
		startingTime = Engine::createEngine()->GetUnpausedTime();
	}
	else
	{
		// Fetches the starting time
		startingTime = Engine::createEngine()->GetTotalTime();
	}
}

/*************************************************************************************************/
/*!
	\brief
		Restarts the timer with the given length and paused status

	\param _length
		The length of the timer

	\param _canBePaused
		Whether the timer pauses when the player is paused
*/
/*************************************************************************************************/
void Timer::RestartTimer(double _length, bool _canBePaused)
{
	// Sets the new variables
	length = _length;
	canBePaused = _canBePaused;

	// Checks if we are using the true time or unpaused time
	if (canBePaused)
	{
		// Fetches the starting time
		startingTime = Engine::createEngine()->GetUnpausedTime();
	}
	else
	{
		// Fetches the starting time
		startingTime = Engine::createEngine()->GetTotalTime();
	}
}

/*************************************************************************************************/
/*!
	\brief
		Gets the time remaining on the timer

	\return
		The time remaining on the timer
*/
/*************************************************************************************************/
double Timer::GetTimeRemaining()
{
	// Checks whether we are checking against paused or unpaused time
	if (canBePaused)
	{
		return std::max(0.0, Engine::createEngine()->GetUnpausedTime() - startingTime + length);
	}
	else
	{
		return std::max(0.0, Engine::createEngine()->GetTotalTime() - startingTime + length);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Gets whether the timer finished

	\return
		Whether the timer finished
*/
/*************************************************************************************************/
bool Timer::Finished()
{
	// Checks whether we are checking against paused or unpaused time
	if (canBePaused)
	{
		return startingTime + length >= Engine::createEngine()->GetUnpausedTime();
	}
	else
	{
		return startingTime + length >= Engine::createEngine()->GetTotalTime();
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
