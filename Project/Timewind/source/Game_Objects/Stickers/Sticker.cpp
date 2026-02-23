/*************************************************************************************************/
/*!
\file Sticker.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.1.30
\brief
    The sticker object and base class

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Sticker.h"
#include "../../Engine/cppShortcuts.h"

// Managers
#include "../../Engine/InputManager.h"

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
		Function for when the player is hovering to place this sticker in a location

	\param tileCoords
		The tile the player is hovering over
*/
/*************************************************************************************************/
void Sticker::Hovering(std::pair<int, int> tileCoords)
{
	// Hovers a faded bumper over the target tile
	SetPosition(ConvertMapCoordsToWorldCoords(tileCoords));
	DrawThisFrame(true);
	SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });

	// Rotates the object
	RotateObject();
}

/*************************************************************************************************/
/*!
	\brief
		Function for rotating the sticker by 45 degrees. Used during placement
*/
/*************************************************************************************************/
void Sticker::RotateObject()
{
	if (canRotate)
	{
		// Checks for rotation commands
		if (_InputManager->CheckInputStatus(InputManager::Inputs::Left) == InputManager::InputStatus::Pressed)
		{
			SetRotation(GetRotation() - 45.0f);
		}
		if (_InputManager->CheckInputStatus(InputManager::Inputs::Right) == InputManager::InputStatus::Pressed)
		{
			SetRotation(GetRotation() + 45.0f);
		}
	}
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
bool Sticker::Place(std::pair<int, int> tileCoords)
{
	return SimplePlace(tileCoords, MapMatrix::TileStatus::Sticker);
}

/*************************************************************************************************/
/*!
	\brief
		Place this object in the scene at the target location. Used for stickers with basic placement behavior

	\param tileCoords
		The tile the player is hovering over

	\param tileStatus
		The status of the tile being placed

	\return
		Whether the sticker was successfully placed
*/
/*************************************************************************************************/
bool Sticker::SimplePlace(std::pair<int, int> tileCoords, MapMatrix::TileStatus tileStatus)
{
	// If the space the camera is looking at is empty
	if (_MapMatrix->GetTile(tileCoords).tileStatus == MapMatrix::TileStatus::Empty)
	{
		// Puts the bumper in the empty space
		_MapMatrix->SetTile(tileCoords, tileStatus, this);
		SetStickerActive(true);
		SetRender(true);
		SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// Returns true that the sticker was placed
		return true;
	}

	// If the sticker was not placed
	return false;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
