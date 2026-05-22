/*************************************************************************************************/
/*!
\file Menu.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.4.27
\brief
    The menu base class

    Functions include:
        + [FILL]

Copyright (c) 2025 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Menu.h"
#include "../Engine/cppShortcuts.h"

// Additional Includes
#include "MenuOptions/MenuOption.h"
#include "../Game_Objects/GameObject.h"
#include "../Game_Objects/Text.h"
#include "../Engine/TextureManager.h"
#include "MenuOptions/SubmenuOption.h"
#include "MenuOptions/QuitOption.h"
#include "../Engine/GameObjectManager.h"
#include "MenuOptions/CreateSubmenuOption.h"

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
Menu::Menu() : menuType(MenuType::Other), optionList(), menuObjects(), optionIndex(0), active(true), vertical(true)
{

}

/*************************************************************************************************/
/*!
	\brief
		Constructs a default menu type (won't do anything if type is other or max)

	\param _menuType
		The default menu type to build
*/
/*************************************************************************************************/
Menu::Menu(MenuType _menuType) : menuType(_menuType), optionList(), menuObjects(), optionIndex(0), active(true), vertical(true), fragile(false)
{
	if (_menuType == MenuType::Pause)
	{
		// Creates the pause menu
		Text* resumeOptionText = new Text("Resume", _TextureManager->GetDefaultFont(), 24, { -2.0f, 2.0f }, 0.0f, { 0.1f, 0.1f }, 90, { 1.0f, 1.0f, 1.0f, 1.0f });
		Text* optionsOptionText = new Text("Options", _TextureManager->GetDefaultFont(), 24, { -2.25f, 0.0f }, 0.0f, { 0.1f, 0.1f }, 90, { 1.0f, 1.0f, 1.0f, 1.0f });
		Text* quitOptionText = new Text("Quit", _TextureManager->GetDefaultFont(), 24, { -1.5f, -2.0f }, 0.0f, { 0.1f, 0.1f }, 90, { 1.0f, 1.0f, 1.0f, 1.0f });
		SubmenuOption* resumeOption = new SubmenuOption(resumeOptionText, this, SubmenuOption::SubmenuInteraction::Close);
		CreateSubmenuOption* quitOption = new CreateSubmenuOption(quitOptionText, MenuType::QuitConfirmation);
		_GameObjectManager->AddGameObject(resumeOptionText);
		_GameObjectManager->AddGameObject(quitOptionText);
		AddOption(resumeOption);
		AddOption(quitOption);
		active = false;
		fragile = true;
	}
	else if (_menuType == MenuType::QuitConfirmation)
	{
		// Creates the main main menu menu
		Text* yesOptionText = new Text("Yes", _TextureManager->GetDefaultFont(), 24, { -3.0f, 0.0f }, 0.0f, { 0.1f, 0.1f }, 90, { 1.0f, 1.0f, 1.0f, 1.0f });
		Text* noOptionText = new Text("No", _TextureManager->GetDefaultFont(), 24, { 0.5f, 0.0f }, 0.0f, { 0.1f, 0.1f }, 90, { 1.0f, 1.0f, 1.0f, 1.0f });
		QuitOption* quitOption = new QuitOption(yesOptionText);
		SubmenuOption* noOption = new SubmenuOption(noOptionText, this, SubmenuOption::SubmenuInteraction::Close);
		_GameObjectManager->AddGameObject(yesOptionText);
		_GameObjectManager->AddGameObject(noOptionText);
		AddOption(quitOption);
		AddOption(noOption);
		active = false;
		vertical = false;
		fragile = true;
	}
}

/*************************************************************************************************/
/*!
	 \brief
		Destructor for menu class
*/
/*************************************************************************************************/
Menu::~Menu()
{
	// Destroys each option
	for (auto i = optionList.begin(); i != optionList.end(); i++)
	{
		delete (*i);
	}

	// Marks all the objects for destruction
	for (auto i = menuObjects.begin(); i != menuObjects.end(); i++)
	{
		(*i)->SetToBeDestroyed(true);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Adds the given key to the inventory

	\param newKey
		The new key to be added into the inventory

	\return
		Returns true if the key was added. False if not
*/
/*************************************************************************************************/
bool Menu::AddOption(MenuOption* newOption)
{
	// Error checking
	if (newOption)
	{
		optionList.push_back(newOption);
		return true;
	}

	// If error checking failed, returns false
	return false;
}

/*************************************************************************************************/
/*!
	\brief
		Adds the given object to the menu objects list

	\param newObject
		The new object to be added into the menu objects list
*/
/*************************************************************************************************/
void Menu::AddMenuObject(GameObject* newObject)
{
	// Error checking
	if (newObject)
	{
		menuObjects.push_back(newObject);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Returns the sticker at the selected index

	\return
		The chosen sticker
*/
/*************************************************************************************************/
MenuOption* Menu::GetSelectedOption()
{
	// Checks that there are options
	if (optionIndex < optionList.size())
	{
		return optionList[optionIndex];
	}

	// Otherwise returns NULL
	return NULL;
}

/*************************************************************************************************/
/*!
	\brief
		Increments the selected menu option
*/
/*************************************************************************************************/
void Menu::IncrementOptionIndex()
{
	// Increments the option index
	optionIndex++;

	// Checks if we overflow
	if (optionIndex >= optionList.size())
	{
		optionIndex = 0;
	}
}

/*************************************************************************************************/
/*!
	\brief
		Decrements the selected menu option
*/
/*************************************************************************************************/
void Menu::DecrementOptionIndex()
{
	// Decrements the option index
	optionIndex--;

	// Checks if we overflow
	if (optionIndex < 0)
	{
		optionIndex = optionList.size();
	}
}

/*************************************************************************************************/
/*!
	\brief
		Turns on the menu and all associated objects
*/
/*************************************************************************************************/
void Menu::TurnOnMenu()
{
	// Loops through the option list
	for (auto i = optionList.begin(); i != optionList.end(); i++)
	{
		// Turns on the option's visual
		(*i)->GetVisual()->SetActive(true);
	}

	// Loops through the objects list
	for (auto i = menuObjects.begin(); i != menuObjects.end(); i++)
	{
		// Turns on the objects
		(*i)->SetActive(true);
	}

	// Sets the menu to on
	active = true;
}

/*************************************************************************************************/
/*!
	\brief
		Turns off the menu and all associated objects and returns true. If menu is fragile, will delete and return false instead

	\param resetIndex
		Whether to reset the option index

	\return
		Returns true if the menu is now dormant. Returns false if the menu was destroyed
*/
/*************************************************************************************************/
bool Menu::TurnOffMenu(bool resetIndex)
{
	// If the menu is persistent
	if (!fragile)
	{
		// Loops through the option list
		for (auto i = optionList.begin(); i != optionList.end(); i++)
		{
			// Turns off the option's visual
			(*i)->GetVisual()->SetActive(false);
		}

		// Loops through the objects list
		for (auto i = menuObjects.begin(); i != menuObjects.end(); i++)
		{
			// Turns off the objects
			(*i)->SetActive(false);
		}

		// Resets the index if requested
		if (resetIndex)
		{
			optionIndex = 0;
		}

		// Sets the menu to off
		active = false;
		return true;
	}
	// If the menu is fragile, destroy it
	else
	{
		delete this;
		return false;
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
