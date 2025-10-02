/*************************************************************************************************/
/*!
\file ScreenCrack.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2025.7.21
\brief
    The destructible wall object

    Public Functions:
        + FILL
		
	Private Functions:
		+ FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_ScreenCrack_H_
#define Syncopatience_ScreenCrack_H_

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
		The screen crack game object class
*/
/*************************************************************************************************/
class ScreenCrack : public GameObject
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
	ScreenCrack(glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, bool facingRight_, Texture* texture_, glm::vec4 color_, float crackDepth_) : GameObject(pos, rot, sca, drawPriority_, facingRight_, texture_, color_), crackDepth(crackDepth_) {}
	ScreenCrack(glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, bool facingRight_, glm::vec4 color_, float crackDepth_) : GameObject(pos, rot, sca, drawPriority_, facingRight_, color_), crackDepth(crackDepth_) {}
	
	/*************************************************************************************************/
	/*!
		\brief
			Destructor for destructible wall class. If there is an item in the wall, will put that item in the map.
	*/
	/*************************************************************************************************/
	virtual ~ScreenCrack() {}

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
			Returns the transformation matrix for the game object

		\return
			The game object's transformation matrix
	*/
	/*************************************************************************************************/
	virtual glm::mat4x4 GetTranformationMatrix();
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------

	float crackDepth;							// How far the crack is in the z direction
	
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

#endif // Syncopatience_ScreenCrack_H_
