/*************************************************************************************************/
/*!
\file LockedWall.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2025.5.11
\brief
    The locked wall game object

    Functions include:
        + FILL

Copyright (c) 2025 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_LockedWall_H_
#define Syncopatience_LockedWall_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"

#include "GameObject.h"

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
		The locked wall game object class
*/
/*************************************************************************************************/
class LockedWall : public GameObject
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
  
	/*********************************************************************************************/
	/*!
		\brief
			Constructor for the locked wall game object class

		\param keyValue_
			The value of the key needed to open the lock

		\param insideObject_
			The object inside the wall that will pop out when destroyed. Set to null if nothing is supposed to pop out.

		\param pos
			The position of the game object

		\param rot
			The rotation of the game object

		\param sca
			The scale of the game object

		\param drawPriority_
			Higher draw priorities are drawn in front of objects with lower priority

		\param inMap_
			Whether this game object is in the map

		\param color_
			The color of the game object, defaults to clear

		\param mapCoords_
			The map coordinates that the game object is in
	*/
	/*********************************************************************************************/
	LockedWall(int keyValue_, glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, bool facingRight_, Texture* texture_, glm::vec4 color_, std::pair<int, int> mapCoords_) : GameObject(pos, rot, sca, drawPriority_, facingRight_, texture_, color_, mapCoords_), keyValue(keyValue_) {}
	LockedWall(int keyValue_, int drawPriority_, Texture* texture_, glm::vec4 color_, std::pair<int, int> mapCoords_) : GameObject(drawPriority_, texture_, color_, mapCoords_), keyValue(keyValue_) {}
  
	/*********************************************************************************************/
	/*!
		\brief
			Destructor for locked wall game object class
	*/
	/*********************************************************************************************/
	virtual ~LockedWall() {}

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
	virtual void Update(double dt, InputManager* inputManager) {}

	/*************************************************************************************************/
	/*!
		\brief
			Gets the door's key value

		\return
			The key value
	*/
	/*************************************************************************************************/
	virtual int GetKeyValue() { return keyValue; }
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------

	int keyValue;								// The key needed to unlock the door
	
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

#endif // Syncopatience_LockedWall_H_
