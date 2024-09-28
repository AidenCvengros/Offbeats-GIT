/*************************************************************************************************/
/*!
\file MapMatrix.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.6.21
\brief
    Manages the world map grid

    Public Functions:
        + System
		+ ~System
		+ Init
		+ Update
		+ Draw
		+ Shutdown
		
	Private Functions:

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_MapMatrix_H_
#define Syncopatience_MapMatrix_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"

// The system base class
#include "System.h"

// The vectors that make up the map grid
#include <vector>

// The game object class
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
		The map matrix class
*/
/*************************************************************************************************/
class MapMatrix : public System
{
public:
	//---------------------------------------------------------------------------------------------
	// Public Consts
	//---------------------------------------------------------------------------------------------

	enum class TileStatus
	{
		Empty,									// The player can walk through spaces on this side ^^^
		Player,
		Enemy,									// The player cannot walk through spaces on this side vvv
		Destructible,
		Wall,
		Max
	};

	//---------------------------------------------------------------------------------------------
	// Public Structures
	//---------------------------------------------------------------------------------------------

	typedef struct
	{
		TileStatus tileStatus;
		GameObject* tileObject;
	}MapTile;

	//---------------------------------------------------------------------------------------------
	// Public Variables
	//---------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------------
	// Public Function Declarations
	//---------------------------------------------------------------------------------------------

	/*************************************************************************************************/
	/*!
		\brief
			Constructor for the map matrix class

		\param
			The max width for maps

		\param
			The max height for maps
	*/
	/*************************************************************************************************/
	MapMatrix(int width, int height);

	/*************************************************************************************************/
	/*!
		\brief
			Destructor for the map matrix class
	*/
	/*************************************************************************************************/
	~MapMatrix();

	/*************************************************************************************************/
	/*!
		\brief
			Initializes the map matrix
	*/
	/*************************************************************************************************/
	void Init();

	/*************************************************************************************************/
	/*!
		\brief
			Updates the system. Virtual function that must be overwritten by derived class
	
		\param
			The time elapsed since the previous frame.
	*/
	/*************************************************************************************************/
	void Update(double dt) {}

	/*************************************************************************************************/
	/*!
		\brief
			Draws the map. Only used for debug purposes

		\param window
			The window being drawn to
	*/
	/*************************************************************************************************/
	void Draw(Window* window);

	/*************************************************************************************************/
	/*!
		\brief
			Shuts down the map matrix system
	*/
	/*************************************************************************************************/
	void Shutdown();

	/*************************************************************************************************/
	/*!
		\brief
			Clears the contents of the map
	*/
	/*************************************************************************************************/
	void Clear();

	/*************************************************************************************************/
	/*!
		\brief
			Sets the value of a given tile position

		\param xCoord
			The x coordinate of the tile to be changed

		\param yCoord
			The y coordinate of the tile to be changed

		\param newStatus
			The new state of the tile

		\param gameObject
			The game object associated with the tile. Defaults to null if there's not meant to be a game object
	*/
	/*************************************************************************************************/
	void SetTile(int xCoord, int yCoord, TileStatus newStatus, GameObject* gameObject = NULL);

	/*************************************************************************************************/
	/*!
		\brief
			Sets the value of a given tile position

		\param coordinates
			The coordinates of the given tile

		\param newStatus
			The new state of the tile

		\param gameObject
			The game object associated with the tile. Defaults to null if there's not meant to be a game object
	*/
	/*************************************************************************************************/
	void SetTile(std::pair<int, int> coordinates, TileStatus newStatus, GameObject* gameObject = NULL) { SetTile(coordinates.first, coordinates.second, newStatus, gameObject); }

	/*************************************************************************************************/
	/*!
		\brief
			Moves the player to a specific position. Returns false if the move would be illegal (oob, wall, etc.)

		\param xCoord
			The x coordinate of the tile to be changed

		\param yCoord
			The y coordinate of the tile to be changed

		\param playerObject
			The player game object

		\return
			Whether the player was moved successfully
	*/
	/*************************************************************************************************/
	bool SetPlayerPosition(int xCoord, int yCoord, GameObject* playerObject);

	/*************************************************************************************************/
	/*!
		\brief
			Moves the player to a specific position. Returns false if the move would be illegal (oob, wall, etc.)

		\param posCoords
			The new position coordinates for the player

		\param playerObject
			The player game object

		\return
			Whether the player was moved successfully
	*/
	/*************************************************************************************************/
	bool SetPlayerPosition(std::pair<int, int> posCoords, GameObject* playerObject) { return SetPlayerPosition(posCoords.first, posCoords.second, playerObject); }

	/*************************************************************************************************/
	/*!
		\brief
			Moves an object that's on a tile to a new tile. Returns false if the move would be illegal (oob, wall, etc.)

		\param prevXCoord
			The x coordinate of the previous tile

		\param prevYCoord
			The y coordinate of the previous tile

		\param newXCoord
			The x coordinate of the new tile

		\param newYCoord
			The y coordinate of the new tile

		\param tileStatus
			The status of the new tile

		\param object
			The game being moved

		\return
			Whether the object was moved successfully
	*/
	/*************************************************************************************************/
	bool MoveTile(int prevXCoord, int prevYCoord, int newXCoord, int newYCoord, TileStatus tileStatus, GameObject* object);

	/*************************************************************************************************/
	/*!
		\brief
			Moves an object that's on a tile to a new tile. Returns false if the move would be illegal (oob, wall, etc.)

		\param prevTile
			The previous tile the object was on

		\param newTile
			The new tile the object is moving to

		\param object
			The game object being moved

		\param tileStatus
			The status of the new tile

		\return
			Whether the object was moved successfully
	*/
	/*************************************************************************************************/
	bool MoveTile(std::pair<int, int> prevTile, std::pair<int, int> newTile, TileStatus tileStatus, GameObject* object) { return MoveTile(prevTile.first, prevTile.second, newTile.first, newTile.second, tileStatus, object); }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the tile's status

		\param xCoord
			The x coordinate of the tile to be checked

		\param yCoord
			The y coordinate of the tile to be checked

		\return
			The status of the given tile
	*/
	/*************************************************************************************************/
	MapTile GetTile(int xCoord, int yCoord);

	/*************************************************************************************************/
	/*!
		\brief
			Returns the player's position

		\return
			The position of the player in the map grid
	*/
	/*************************************************************************************************/
	std::pair<int, int> GetPlayerPosition();

	/*************************************************************************************************/
	/*!
		\brief
			Returns that maximum width of the map grid

		\return
			The maximum width of the map grid
	*/
	/*************************************************************************************************/
	int GetMaxMapWidth();

	/*************************************************************************************************/
	/*!
		\brief
			Returns that maximum height of the map grid

		\return
			The maximum height of the map grid
	*/
	/*************************************************************************************************/
	int GetMaxMapHeight();

	/*************************************************************************************************/
	/*!
		\brief
			Returns that farthest left edge of the map

		\return
			The left-most edge of the map in world coordinates
	*/
	/*************************************************************************************************/
	float GetMinMapX() { return minX; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns that lowest edge of the map

		\return
			The lowest of the map in world coordinates
	*/
	/*************************************************************************************************/
	float GetMinMapY() { return minY; }

	/*************************************************************************************************/
	/*!
		\brief
			Finds the coordinates of a tile associated with the given game object

		\param xCoord
			The x coordinate of the tile to be changed

		\param yCoord
			The y coordinate of the tile to be changed

		\param newStatus
			The new state of the tile

		\param gameObject
			The game object associated with the tile. Defaults to null if there's not meant to be a game object
	*/
	/*************************************************************************************************/
	//std::pair<int, int> FindTileCoords(GameObject* gameObject);

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

	std::vector<std::vector<MapTile>> mapMatrix;	// The map grid

	std::pair<int, int> playerPos;											// The position of the player

	float minX;																// The farthest left you can go
	float minY;																// The farthest down you can go (y is inverted btw)

	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

	/*************************************************************************************************/
	/*!
		\brief
			Checks if the given coordinates are within the map

		\param xCoord
			The x coordinate

		\param yCoord
			The y coordinate

		\return
			True if the coordinates are within the map, false otherwise
	*/
	/*************************************************************************************************/
	bool ValidateCoordinates(int xCoord, int yCoord);
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_MapMatrix_H_
