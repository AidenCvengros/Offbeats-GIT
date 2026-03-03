/*************************************************************************************************/
/*!
\file BlockSticker.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.2.24
\brief
    The block sticker object class

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Teleporter.h"
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
*/
/*************************************************************************************************/
void Teleporter::Update(double dt)
{
	
}

/*************************************************************************************************/
/*!
	\brief
		Function for when the player is hovering to place this sticker in a location

	\param tileCoords
		The tile the player is hovering over
*/
/*************************************************************************************************/
void Teleporter::Hovering(std::pair<int, int> tileCoords)
{
	// Hovers a faded bumper over the target tile
	SetPosition(ConvertMapCoordsToWorldCoords(tileCoords));
	DrawThisFrame(true);
	SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });
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
bool Teleporter::Place(std::pair<int, int> tileCoords)
{
	// Uses simple place
	return SimplePlace(tileCoords, MapMatrix::TileStatus::Teleporter);
}

/*************************************************************************************************/
/*!
	\brief
		Turns on the teleporter

	\param newActiveStatus
		The new active boolean for the teleporter to be set to
*/
/*************************************************************************************************/
void Teleporter::SetTeleporterActive(bool newActiveStatus)
{
	teleporterActive = newActiveStatus;

	// Changes color depending on whether it's active
	if (teleporterActive)
	{
		SetColor({ 0.2f, 0.4f, 0.9f, 0.5f });
	}
	else
	{
		SetColor({ 0.6f, 0.6f, 0.6f, 0.25f });
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
