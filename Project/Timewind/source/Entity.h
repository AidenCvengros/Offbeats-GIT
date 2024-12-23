/*************************************************************************************************/
/*!
\file Entity.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2023.9.8
\brief
    Holds the base entity class that makes up non-static or interactible objects on the map

    Public Functions:
        + Entity::Entity
		+ Entity::~Entity
		+ Entity::SetPosition
		+ Entity::GetPosition
		
	Private Functions:
		+ FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_Entity_H_
#define Syncopatience_Entity_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

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
		The base entity class for objects on the map
*/
/*************************************************************************************************/
class Entity
{
public:
	//---------------------------------------------------------------------------------------------
	// Public Consts
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Structures
	//---------------------------------------------------------------------------------------------
	
	struct PosVec
	{
		float x; // The x position
		float y; // The y position
	};

	//---------------------------------------------------------------------------------------------
	// Public Variables
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Function Declarations
	//---------------------------------------------------------------------------------------------

	/*************************************************************************************************/
	/*!
		\brief
			Constructor for the entity class
	*/
	/*************************************************************************************************/
	Entity(float _posX, float _posY);

	/*************************************************************************************************/
	/*!
		\brief
			Destructor for the entity class
	*/
	/*************************************************************************************************/
	~Entity();

	/*************************************************************************************************/
	/*!
		\brief
			Sets the current position vector

		\param newPosition
			The new position for this entity
	*/
	/*************************************************************************************************/
	void SetPosition(Entity::PosVec newPosition);

	/*************************************************************************************************/
	/*!
		\brief
			Sets the current position vector

		\param newXPos
			The new X position for this entity

		\param newYPos
			The new Y position for this entity
	*/
	/*************************************************************************************************/
	void SetPosition(float newXPos, float newYPos);

	/*************************************************************************************************/
	/*!
		\brief
			Returns the current position vector

		\return
			The position of this entity
	*/
	/*************************************************************************************************/
	Entity::PosVec GetPosition();
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------
	
	PosVec position;							// The position of the entity

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

#endif // Syncopatience_Entity_H_
