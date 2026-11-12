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

	\param startRunning
		Whether the timer starts running as soon as it is created
*/
/*************************************************************************************************/
Timer::Timer(double _length, bool _canBePaused, bool startRunning) : length(_length), fullLength(_length), canBePaused(_canBePaused), running(startRunning)
{
	SetStartingTime();
}

/*************************************************************************************************/
/*!
	\brief
		Starts a paused timer, or restarts a finished timer with the previous values. Does nothing if the timer is already running.
*/
/*************************************************************************************************/
void Timer::Start()
{
	// Checks that we were paused in the first place or if the timer finished
	if (!running || Finished())
	{
		// Sets the new starting time
		SetStartingTime();
		length = fullLength;
	}

	// Marks the timer as running
	running = true;
}

/*************************************************************************************************/
/*!
	\brief
		Pause the timer
*/
/*************************************************************************************************/
void Timer::Pause()
{
	// Adjusts the length
	length = GetTimeRemaining();

	// Pauses the timer
	running = false;
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
void Timer::Restart(double _length, bool _canBePaused)
{
	// Sets the new variables
	length = _length;
	fullLength = _length;
	canBePaused = _canBePaused;

	// Starts the timer
	SetStartingTime();
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
	// If it's running
	if (running)
	{
		// Checks whether we are checking against paused or unpaused time
		if (canBePaused)
		{
			return std::max(0.0, length - (Engine::createEngine()->GetUnpausedTime() - startingTime));
		}
		else
		{
			return std::max(0.0, length - (Engine::createEngine()->GetTotalTime() - startingTime));
		}
	}

	// Otherwise returns the saved time remaining
	return length;
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
	// Checks whether the timer isn't paused (or wasn't paused after we finished anyways)
	if (running && length > 0.0)
	{
		// Checks whether we are checking against paused or unpaused time
		if (canBePaused)
		{
			return startingTime + length < Engine::createEngine()->GetUnpausedTime();
		}
		else
		{
			return startingTime + length < Engine::createEngine()->GetTotalTime();
		}
	}
	else
	{
		// If we are paused, then we aren't done
		return false;
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Sets the starting time (will differ based on whether we pause the timer with the game or not)
*/
/*************************************************************************************************/
void Timer::SetStartingTime()
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
