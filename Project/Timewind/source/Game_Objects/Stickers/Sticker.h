/*************************************************************************************************/
/*!
\file Sticker.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.1.30
\brief
    The sticker object and base class

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

#ifndef Syncopatience_Sticker_H_
#define Syncopatience_Sticker_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "../Item.h"

#include "../../Gameplay/MapMatrix.h"

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
class Sticker : public Item
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
			Constructor for the sticker game object class

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
	Sticker(Item::ItemType itemType, glm::vec2 pos, float rot, glm::vec2 sca, bool facingRight_, Texture* texture_, glm::vec4 color_, std::pair<int, int> mapCoords_) : Item(itemType, pos, rot, sca, 40, facingRight_, texture_, color_, mapCoords_), stickerActive(false), canRotate(true) {}
	Sticker(Item::ItemType itemType, Texture* texture_, glm::vec4 color_, std::pair<int, int> mapCoords_) : Item(itemType, 40, texture_, color_, mapCoords_), stickerActive(false), canRotate(true) {}

	/*************************************************************************************************/
	/*!
		\brief
			Destructor for the sticker class
	*/
	/*************************************************************************************************/
	~Sticker() {}

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
			Function for rotating the sticker by 45 degrees. Used during placement
	*/
	/*************************************************************************************************/
	void RotateObject();

	/*************************************************************************************************/
	/*!
		\brief
			Place this object in the scene at the target location

		\param tileCoords
			The tile the player is hovering over

		\return
			Whether the sticker was successfully placed
	*/
	/*************************************************************************************************/
	virtual bool Place(std::pair<int, int> tileCoords);

	/*************************************************************************************************/
	/*!
		\brief
			Place this object in the scene at the target location. Used for stickers with basic placement behavior

		\param tileCoords
			The tile the player is hovering over

		\param tileStatus
			The status of the tile being placed

		\return
			Whether the sticker was successfully placed
	*/
	/*************************************************************************************************/
	bool SimplePlace(std::pair<int, int> tileCoords, MapMatrix::TileStatus tileStatus);

	/*************************************************************************************************/
	/*!
		\brief
			Returns whether this sticker is currently active

		\return
			The bumper strength
	*/
	/*************************************************************************************************/
	bool GetStickerActive() { return stickerActive; }

	/*************************************************************************************************/
	/*!
		\brief
			Sets whether this sticker is currently active

		\param newStickerActive
			The new sticker active status
	*/
	/*************************************************************************************************/
	void SetStickerActive(bool newStickerActive) { stickerActive = newStickerActive; }

	/*************************************************************************************************/
	/*!
		\brief
			Sets whether this sticker can be rotated during placement

		\param newCanRotate
			Whether this sticker can be rotated during placement
	*/
	/*************************************************************************************************/
	void SetCanRotate(bool newCanRotate) { canRotate = newCanRotate; }
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------

	bool stickerActive;							// Whether the sticker can be picked up (false) or is active (true)
	bool canRotate;								// Whether the player can rotate the sticker while hovering

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

#endif // Syncopatience_Sticker_H_
