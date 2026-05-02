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
Menu::Menu() : optionList(), menuObjects(), optionIndex(0), active(true), vertical(true)
{

}

/*************************************************************************************************/
/*!
	 \brief
		Destructor for menu class
*/
/*************************************************************************************************/
Menu::~Menu()
{

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
		Turns off the menu and all associated objects

	\param resetIndex
		Whether to reset the option index
*/
/*************************************************************************************************/
void Menu::TurnOffMenu(bool resetIndex)
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
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
