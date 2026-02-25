/*************************************************************************************************/
/*!
\file BlockSticker.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.2.24
\brief
    The block sticker object

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

#ifndef Syncopatience_BlockSticker_H_
#define Syncopatience_BlockSticker_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "Sticker.h"

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
class BlockSticker : public Sticker
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
			Constructor for the block sticker game object class

		\param pos
			The position of the game object

		\param rot
			The rotation of the game object

		\param sca
			The scale of the game object

		\param inMap_
			Whether this game object is in the map

		\param color_
			The color of the game object, defaults to clear

		\param mapCoords_
			The map coordinates that the game object is in
	*/
	/*************************************************************************************************/
	BlockSticker(glm::vec2 pos, float rot, glm::vec2 sca, bool facingRight_, Texture* texture_, glm::vec4 color_, std::pair<int, int> mapCoords_) : Sticker(Item::ItemType::BlockSticker, pos, rot, sca, facingRight_, texture_, color_, mapCoords_), sand(false), regenerateTimer(0.0) {}
	BlockSticker(Texture* texture_, glm::vec4 color_, std::pair<int, int> mapCoords_) : Sticker(Item::ItemType::BlockSticker, texture_, color_, mapCoords_), sand(false), regenerateTimer(0.0) {}

	/*************************************************************************************************/
	/*!
		\brief
			Destructor for the bumper class
	*/
	/*************************************************************************************************/
	~BlockSticker() {}

	/*************************************************************************************************/
	/*!
		\brief
			Updates the game object. Can be overwritten by derived classes

		\param dt
			The time elapsed since the previous frame
	*/
	/*************************************************************************************************/
	virtual void Update(double dt);

	/*************************************************************************************************/
	/*!
		\brief
			Function for when the player is hovering to place this sticker in a location

		\param tileCoords
			The tile the player is hovering over
	*/
	/*************************************************************************************************/
	virtual void Hovering(std::pair<int, int> tileCoords);

	/*************************************************************************************************/
	/*!
		\brief
			Place this object in the scene at the target location
	
		\param tileCoords
			The tile the player is hovering over
	
		\return
			Whether the sticker was succesfully placed
	*/
	/*************************************************************************************************/
	virtual bool Place(std::pair<int, int> tileCoords);
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------

	bool sand;									// Whether the block breaks shortly after contact (will regenerate)
	double regenerateTimer;						// The timer to regenerate the block

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

#endif // Syncopatience_BlockSticker_H_
