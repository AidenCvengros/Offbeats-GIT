/*************************************************************************************************/
/*!
\file ChildObject.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2025.3.17
\brief
    The base class for child game objects

    Public Functions:
        + FILL
		
	Private Functions:
		+ FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_ChildObject_H_
#define Syncopatience_ChildObject_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"

#include "GameObject.h"

#include "MapMatrix.h"

#include "AttackManager.h"

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
		The child game object class
*/
/*************************************************************************************************/
class ChildObject : public GameObject
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
			Constructor for the child game object class
			
		\param parentObject_
			The parent game object

		\param flipWithParent_
			Whether the child should flip direction and sides when the parent flips
	*/
	/*************************************************************************************************/
	ChildObject(GameObject* parentObject_, bool flipWithParent_) : GameObject(parentObject_->GetPosition(), parentObject_->GetRotation(), parentObject_->GetScale(), parentObject_->GetDrawPriority(), parentObject_->GetIsFacingRight(), parentObject_->GetColor()),
		parentObject(parentObject_), flipWithParent(flipWithParent_) {}
	
	/*************************************************************************************************/
	/*!
		\brief
			Destructor for FILL class
	*/
	/*************************************************************************************************/
	~ChildObject() {}

	/*************************************************************************************************/
	/*!
		\brief
			Returns the transformation matrix for the game object

		\return
			The game object's transformation matrix
	*/
	/*************************************************************************************************/
	glm::mat4x4 GetTransformationMatrix();
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Structures
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Variables
	//---------------------------------------------------------------------------------------------
	
	GameObject* parentObject;					// The parent object that this child is attached to
	bool flipWithParent;						// Flips the child object when the parent object flips direction

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

#endif // Syncopatience_ChildObject_H_
