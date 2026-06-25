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
#include "Menu.h"
#include "MenuOptions/MenuOption.h"
#include "../Engine/InputManager.h"
#include "../Engine/GameStateManager.h"

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
Inventory::Inventory() : keyList(), keyCount(0), coinCount(0), bigCoinCount(0), stickerList(), inventoryObjects(), activeTeleporter(NULL)
{
	// Creates the text object
	coinText = new Text("x00", _TextureManager->GetDefaultFont(), 12, { 12.0f, 10.0f }, 0.0f, { 0.1f, 0.1f }, 90, { 1.0f, 1.0f, 1.0f, 1.0f });
	_GameObjectManager->AddGameObject(coinText);
	coinText->SetFollowingCamera(true);

	// Creates the coin icon
	Texture* coinTexture = _TextureManager->AddTexture("Assets/Sprites/Coin.png");
	GameObject* coinIcon = new GameObject({ 11.5f, 10.4f }, 0.0f, { 1.0f, 1.0f }, 90, true, coinTexture, { 1.0f, 1.0f, 1.0f, 1.0f });
	_GameObjectManager->AddGameObject(coinIcon);
	coinIcon->SetFollowingCamera(true);

	// Creates the sticker inventory menu
	stickerMenu = new Menu();
	stickerMenu->SetIsVertical(false);
	stickerMenu->TurnOffMenu();
	for (int i = 0; i < 10; i++)
	{
		inventoryObjects[i] = new GameObject({ -9.0f + (i * 2.0f), -8.0f }, 0.0f, { 2.0f, 2.0f }, 90, true, { 1.0f, 1.0f, 1.0f, 1.0f });
		inventoryObjects[i]->SetRender(false);
		inventoryObjects[i]->SetFollowingCamera(true);
		inventoryObjects[i]->SetActive(false);
		_GameObjectManager->AddGameObject(inventoryObjects[i]);
		MenuOption* newMenuOption = new MenuOption(inventoryObjects[i]);
		stickerMenu->AddOption(newMenuOption);
	}
	Texture* highlightBoxTexture = _TextureManager->AddTexture("Assets/Sprites/Highlight_Box.png");
	GameObject* highlightBox = new GameObject({ 0.0f, 0.0f }, 0.0f, { 2.0f, 2.0f }, 95, true, highlightBoxTexture, { 0.95f, 0.3f, 0.3f, 1.0f });
	highlightBox->SetFollowingCamera(true);
	highlightBox->SetActive(false);
	_GameObjectManager->AddGameObject(highlightBox);
	stickerMenu->SetCursorObject(highlightBox, { 0.0f, 0.0f });
	Texture* inventorySlotsTexture = _TextureManager->AddTexture("Assets/Sprites/Inventory_Boxes_Pixel.png");
	GameObject* inventorySlots = new GameObject({ 0.0f, -8.0f }, 0.0f, { 20.0f, 2.0f }, 90, true, inventorySlotsTexture, { 1.0f, 1.0f, 1.0f, 1.0f });
	inventorySlots->SetFollowingCamera(true);
	inventorySlots->SetActive(false);
	_GameObjectManager->AddGameObject(inventorySlots);
	stickerMenu->AddMenuObject(inventorySlots);
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
		// Adds in the new key
		keyList[newKey->GetKeyValue()] = newKey;
		keyCount++;

		// Creates the key icon
		Texture* keyTexture = _TextureManager->AddTexture("Assets/Sprites/Key.png");
		GameObject* keyIcon = new GameObject({ 10.0f - (1.5f * keyCount), 10.4f}, 0.0f, {2.0f, 2.0f}, 90, true, keyTexture, newKey->GetColor());
		_GameObjectManager->AddGameObject(keyIcon);
		keyIcon->SetFollowingCamera(true);

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
			inventoryObjects[i]->SetTexture(newSticker->GetTexture());
			inventoryObjects[i]->SetRender(1);
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
	newString << "x" << std::setw(2) << std::setfill('0') << coinCount;
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

	// Creates the coin icon
	Texture* coinTexture = _TextureManager->AddTexture("Assets/Sprites/Coin.png");
	GameObject* coinIcon = new GameObject({ 10.0f, 10.4f }, 0.0f, { 2.0f, 2.0f }, 90, true, coinTexture, { 1.0f, 1.0f, 1.0f, 1.0f });
	_GameObjectManager->AddGameObject(coinIcon);
	coinIcon->SetFollowingCamera(true);
}

/*************************************************************************************************/
/*!
	\brief
		Returns the sticker at the selected index

	\return
		The chosen sticker
*/
/*************************************************************************************************/
Sticker* Inventory::GetSelectedSticker()
{
	return stickerList[stickerMenu->GetOptionIndex()];
}

/*************************************************************************************************/
/*!
	\brief
		Clears the sticker at the selected index
*/
/*************************************************************************************************/
void Inventory::ClearSelectedSticker()
{
	// Clears the selected sticker
	int optionIndex = stickerMenu->GetOptionIndex();
	stickerList[optionIndex] = NULL;
	inventoryObjects[optionIndex]->SetRender(0);
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

/*************************************************************************************************/
/*!
	\brief
		Turns on or off placing mode

	\param isPlacing
		Whether placing mode is on or off
*/
/*************************************************************************************************/
void Inventory::PlacingMode(bool isPlacing)
{
	// If placing mode is on
	if (isPlacing)
	{
		// Turns on the ui if it wasn't already
		if (!stickerMenu->GetActive())
		{
			stickerMenu->TurnOnMenu();
			_GameStateManager->SetCurrentMenu(stickerMenu, true);
		}
	}
	// If placing mode is now getting turned off
	else
	{
		// Turns off the ui if it wasn't already
		if (stickerMenu->GetActive())
		{
			stickerMenu->TurnOffMenu(false);
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
