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

	\return
		Returns true if coin count hit 100.
*/
/*************************************************************************************************/
bool Inventory::AddCoin()
{
	// Adds a coin to the count
	coinCount++;

	return false;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
