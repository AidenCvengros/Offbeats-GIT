/*************************************************************************************************/
/*!
\file Inventory.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2025.5.5
\brief
    The player's inventory manager

    Functions include:
        + [FILL]

Copyright (c) 2025 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_Inventory_H_
#define Syncopatience_Inventory_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"

#include "Game_Objects/Key.h"

//-------------------------------------------------------------------------------------------------
// Forward References
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Constants
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Classes
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		The inventory manager class
*/
/*************************************************************************************************/
class Inventory
{
public:
	//---------------------------------------------------------------------------------------------
	// Public Consts
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Structures
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Variables
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Function Declarations
	//---------------------------------------------------------------------------------------------
	
	/*************************************************************************************************/
	/*!
		\brief
		  Constructor for the inventory class
	*/
	/*************************************************************************************************/
	Inventory() : itemList(NULL) {}

	/*************************************************************************************************/
	/*!
	  \brief
		Destructor for inventory class
	*/
	/*************************************************************************************************/
	~Inventory() {}

	/*************************************************************************************************/
	/*!
	  \brief
		Adds the given item to the inventory
	*/
	/*************************************************************************************************/
	void AddItem(Item* newItem) { itemList.push_back(newItem); }

private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------

	std::vector<Item*> itemList;
	
	//---------------------------------------------------------------------------------------------
	// Private Structures
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Variables
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_Inventory_H_
