/*************************************************************************************************/
/*!
\file LockedWall.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2025.5.11
\brief
    FILL

    Functions include:
        + FILL

Copyright (c) 2025 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "LockedWall.h"
#include "../Engine/cppShortcuts.h"

// Additional includes
#include "../Gameplay/MapMatrix.h"

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

/*********************************************************************************************/
/*!
	\brief
		Constructor for the linked locked wall game object class

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

	\param linkedWall
		Lets the program know this is a linked wall
*/
/*********************************************************************************************/
LockedWall::LockedWall(int keyValue_, glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, bool facingRight_, Texture* texture_, glm::vec4 color_, std::pair<int, int> mapCoords_, bool linkedWall) : GameObject(pos, rot, sca, drawPriority_, facingRight_, texture_, color_, mapCoords_), keyValue(keyValue_), linked(true)
{
	// Adjusts the physical position of the object
	SetPosition({ GetPosition().x, GetPosition().y - 3.0f });
}

/*********************************************************************************************/
/*!
	\brief
		Constructor for the linked locked wall game object class

	\param keyValue_
		The value of the key needed to open the lock

	\param drawPriority_
		Higher draw priorities are drawn in front of objects with lower priority

	\param color_
		The color of the game object, defaults to clear

	\param mapCoords_
		The map coordinates that the game object is in

	\param linkedWall
		Lets the program know this is a linked wall
*/
/*********************************************************************************************/
LockedWall::LockedWall(int keyValue_, int drawPriority_, Texture* texture_, glm::vec4 color_, std::pair<int, int> mapCoords_, bool linkedWall) : GameObject(drawPriority_, texture_, color_, mapCoords_), keyValue(keyValue_), linked(true)
{
	// Adjusts the physical position of the object
	SetPosition({ GetPosition().x, GetPosition().y - 3.0f });
}

/*************************************************************************************************/
/*!
	\brief
		Sets whether the game object should be destroyed

	\param newToBeDestroyed
		Whether the game object should be destroyed
*/
/*************************************************************************************************/
void LockedWall::SetToBeDestroyed(bool newToBeDestroyed)
{
	// Sets this to be destroyed like usual
	GameObject::SetToBeDestroyed(newToBeDestroyed);

	// Checks if this wall is linked
	if (linked)
	{
		// Clears all associated tiles
		for (int i = 0; i < 4; i++)
		{
			_MapMatrix->ClearTile(GetMapCoords().first, GetMapCoords().second - i, false);
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Declarations
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Function Definitions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
