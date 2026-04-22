/*************************************************************************************************/
/*!
\file Inventory.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2025.5.7
\brief
    Manages the player's inventory

    Functions include:
        + [FILL]

Copyright (c) 2025 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Inventory.h"
#include "../Engine/cppShortcuts.h"

// Includes the key class so we can add keys
#include "../Game_Objects/Key.h"
#include "../Game_Objects/Stickers/Sticker.h"
#include "../Game_Objects/Stickers/Teleporter.h"

// Extra includes
#include "../Game_Objects/Text.h"
#include "../Engine/TextureManager.h"
#include "../Engine/GameObjectManager.h"
#include <sstream>
#include <iomanip>

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
		Constructor for the inventory class
*/
/*************************************************************************************************/
Inventory::Inventory() : keyList(), coinCount(0), stickerList(), stickerInventoryCursor(0)
{
	coinText = new Text("x00", _TextureManager->GetDefaultFont(), 12, {12.0f, 10.0f}, 0.0f, {0.1f, 0.1f}, 90, {1.0f, 1.0f, 1.0f, 1.0f});
	_GameObjectManager->AddGameObject(coinText);
	coinText->SetFollowingCamera(true);
}

/*************************************************************************************************/
/*!
	\brief
		Adds the given key to the inventory

	\param newKey
		The new key to be added into the inventory

	\return
		Returns true if the key was added, false if not
*/
/*************************************************************************************************/
bool Inventory::AddKey(Key* newKey)
{
	if (newKey)
	{
		keyList[newKey->GetKeyValue()] = newKey;
		return true;
	}

	// Otherwise return false
	return false;
}

/*************************************************************************************************/
/*!
	\brief
		Adds the given sticker to the inventory

	\param newSticker
		The new sticker to be added into the inventory

	\return
		Returns true if the sticker was added. False if not
*/
/*************************************************************************************************/
bool Inventory::AddSticker(Sticker* newSticker)
{
	// Walks through the sticker list
	for (int i = 0; i < stickerList.size(); i++)
	{
		// If we found an open slot
		if (!stickerList[i])
		{
			// Puts the new sticker in the slot
			newSticker->SetRotation(0.0f);
			stickerList[i] = newSticker;
			return true;
		}
	}

	// If no open slots were found, returns false
	return false;
}

/*************************************************************************************************/
/*!
	\brief
		Adds the given key to the inventory
*/
/*************************************************************************************************/
void Inventory::AddCoin()
{
	// Adds a coin to the count
	coinCount++;

	// Updates the coin text
	std::stringstream newString;
	newString << "x" << std::setw(2) << coinCount;
	coinText->SetText(newString.str());
}

/*************************************************************************************************/
/*!
	\brief
		Adds a big coin to the inventory
*/
/*************************************************************************************************/
void Inventory::AddBigCoin()
{
	// Increments the big coin count
	bigCoinCount++;
}

/*************************************************************************************************/
/*!
	\brief
		Increments the selected sticker one to the right
*/
/*************************************************************************************************/
void Inventory::IncrementSelectedSticker()
{
	// Increments the selected sticker
	stickerInventoryCursor++;

	// Checks if we overflow
	if (stickerInventoryCursor >= stickerList.size())
	{
		stickerInventoryCursor = 0;
	}
}

/*************************************************************************************************/
/*!
	\brief
		Decrements the selected sticker one to the left
*/
/*************************************************************************************************/
void Inventory::DecrementSelectedSticker()
{
	// Decrements the selected sticker
	stickerInventoryCursor--;

	// Checks if we underflow
	if (stickerInventoryCursor < 0)
	{
		stickerInventoryCursor = stickerList.size() - 1;
	}
}

/*************************************************************************************************/
/*!
	\brief
		Sets a new active teleporter

	\param newActiveTeleporter
		The new active teleporter
*/
/*************************************************************************************************/
void Inventory::SetActiveTeleporter(Teleporter* newActiveTeleporter)
{
	// Turns off the old teleporter
	if (activeTeleporter)
	{
		activeTeleporter->SetTeleporterActive(false);
	}

	// Turns on the new teleporter
	activeTeleporter = newActiveTeleporter;
	if (activeTeleporter)
	{
		activeTeleporter->SetTeleporterActive(true);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Teleports the given game object to the teleporter if one is active

	\param gameObject
		The object to be teleported

	\return
		If the object was teleported
*/
/*************************************************************************************************/
bool Inventory::Teleport(GameObject* gameObject)
{
	// Checks that the game object is real and that there is a teleporter active
	if (activeTeleporter && gameObject)
	{
		// Checks that the teleporter is in the map
		if (_MapMatrix->ValidateCoordinates(ConvertWorldCoordsToMapCoords(activeTeleporter->GetPosition())))
		{
			// Teleports the object
			gameObject->SetPosition(activeTeleporter->GetPosition());

			// Success
			return true;
		}
	}

	// Otherwise returns false
	return false;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
