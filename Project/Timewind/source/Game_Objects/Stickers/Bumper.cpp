/*************************************************************************************************/
/*!
\file Bumper.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.1.30
\brief
    The bumper object class

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Bumper.h"
#include "../../Engine/cppShortcuts.h"

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
		Updates the game object. Can be overwritten by derived classes

	\param dt
		The time elapsed since the previous frame

	\param inputManager
		Allows the game objects to check inputs
*/
/*************************************************************************************************/
void Bumper::Update(double dt, InputManager* inputManager)
{

}

/*************************************************************************************************/
/*!
	\brief
		Lets the bumper know it got hit.

	\return
		Returns true if this is the first contact
*/
/*************************************************************************************************/
bool Bumper::Hit()
{
	// Checks if we don't have a bumper timer or if the old one ran out
	if (bumperTimer == NULL || bumperTimer->Finished())
	{
		// Sets up a new timer
		if (bumperTimer)
		{
			bumperTimer->Restart(0.4, true);
		}
		else
		{
			bumperTimer = new Timer(0.4, true);
		}

		// Returns that it was a new hit
		return true;
	}

	// Otherwise returns that this is a continuous hit
	return false;
}

/*************************************************************************************************/
/*!
	\brief
		Place this object in the scene at the target location

	\param tileCoords
		The tile the player is hovering over

	\return
		Whether the sticker was succesfully placed
*/
/*************************************************************************************************/
bool Bumper::Place(std::pair<int, int> tileCoords)
{
	// Uses simple place
	return SimplePlace(tileCoords, MapMatrix::TileStatus::Bumper);
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
