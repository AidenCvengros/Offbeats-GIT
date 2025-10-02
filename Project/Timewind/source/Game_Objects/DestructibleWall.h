/*************************************************************************************************/
/*!
\file DestructibleWall.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2025.5.2
\brief
    The destructible wall object

    Public Functions:
        + FILL
		
	Private Functions:
		+ FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_DestructibleWall_H_
#define Syncopatience_DestructibleWall_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base include
#include "../Engine/stdafx.h"

// The base game object class
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
		The interactible game object class
*/
/*************************************************************************************************/
class DestructibleWall : public GameObject
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
			Constructor for the item game object class

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
	/*************************************************************************************************/
	DestructibleWall(GameObject* insideObject_, glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, bool facingRight_, Texture* texture_, glm::vec4 color_, std::pair<int, int> mapCoords_) : GameObject(pos, rot, sca, drawPriority_, facingRight_, texture_, color_, mapCoords_), insideObject(insideObject_) {}
	DestructibleWall(GameObject* insideObject_, int drawPriority_, Texture* texture_, glm::vec4 color_, std::pair<int, int> mapCoords_) : GameObject(drawPriority_, texture_, color_, mapCoords_), insideObject(insideObject_) {}
	
	/*************************************************************************************************/
	/*!
		\brief
			Destructor for destructible wall class. If there is an item in the wall, will put that item in the map.
	*/
	/*************************************************************************************************/
	virtual ~DestructibleWall();

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
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------

	GameObject* insideObject;							// The type of this item
	
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

#endif // Syncopatience_Item_H_
