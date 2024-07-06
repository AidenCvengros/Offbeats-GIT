/*************************************************************************************************/
/*!
\file MapMatrix.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.6.21
\brief
    Manages the world map grid

    Functions include:
        + System::System
		+ Systme::~System

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "MapMatrix.h"

// Uses game object for drawing the map
#include "GameObject.h"

// Includes window class to be drawn to
#include "Window.h"

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

//-------------------------------------------------------------------------------------------------
// Private Function Declarations
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Function Definitions
//-------------------------------------------------------------------------------------------------

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
MapMatrix::MapMatrix(int width, int height)
{
	// Creates width number of columns
	for (int i = 0; i < width; i++)
	{
		// Makes the columns height tall
		std::vector<TileStatus> column;
		column.resize(height, TileStatus::Empty);
		mapMatrix.push_back(column);
	}

	// Sets the minimum dimensions
	minX = -4.0f;
	minY = 4.0f;
}

/*************************************************************************************************/
/*!
	\brief
		Destructor for the map matrix class
*/
/*************************************************************************************************/
MapMatrix::~MapMatrix()
{

}

/*************************************************************************************************/
/*!
	\brief
		Initializes the map matrix
*/
/*************************************************************************************************/
void MapMatrix::Init()
{
	
}

/*************************************************************************************************/
/*!
	\brief
		Draws the map. Only used for debug purposes

	\param window
		The window being drawn to
*/
/*************************************************************************************************/
void MapMatrix::Draw(Window* window)
{
	// Creates a square game object that's gonna be the drawing
	GameObject debugSquare({ 0.0f, 0.0f }, 0.0f, { 2.0f, 2.0f }, 49, { 0.0f, 0.0f, 0.0f, 0.5f });
	debugSquare.SetRender(true);

	// Draws the map
	for (int i = 0; i < mapMatrix.size(); i++)
	{
		for (int j = 0; j < mapMatrix[i].size(); j++)
		{
			// Boolean to track whether a square should be drawn on this tile
			bool shouldDraw = false;

			// Checks for the player
			switch (mapMatrix[i][j])
			{
			// Does nothing because the spot is empty
			case TileStatus::Empty:
				break;
			// Uses green for the player
			case TileStatus::Player:
				debugSquare.SetColor({ 0.0f, 1.0f, 0.0f, 0.5f });
				shouldDraw = true;
				break;
			// Uses red for enemies
			case TileStatus::Enemy:
				debugSquare.SetColor({ 1.0f, 0.0f, 0.0f, 0.5f });
				shouldDraw = true;
				break;
			// Uses light blue for destructibles
			case TileStatus::Destructible:
				debugSquare.SetColor({ 0.2f, 0.2f, 1.0f, 0.5f });
				shouldDraw = true;
				break;
			// Uses light grey for walls
			case TileStatus::Wall:
				debugSquare.SetColor({ 0.6f, 0.6f, 0.6f, 0.5f });
				shouldDraw = true;
				break;
			default:
				break;
			}

			// Checks if the square should be drawn
			if (shouldDraw)
			{
				debugSquare.SetPosition({ minX + (2.0f * i), minY - (2.0f * j) });
				window->DrawGameObject(debugSquare);
			}
		}
	}
}

/*************************************************************************************************/
/*!
	\brief
		Shuts down the map matrix system
*/
/*************************************************************************************************/
void MapMatrix::Shutdown()
{
	// Clears the map
	Clear();
}

/*************************************************************************************************/
/*!
	\brief
		Clears the contents of the map
*/
/*************************************************************************************************/
void MapMatrix::Clear()
{
	// Loops through the map columns
	for (int i = 0; i < mapMatrix.size(); i++)
	{
		// Clears the contents of the column, then refills it with empty tiles
		mapMatrix[i].clear();
		mapMatrix[i].resize(mapMatrix.capacity(), TileStatus::Empty);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Clears the contents of the map

	\param xCoord
		The x coordinate of the tile to be changed

	\param yCoord
		The y coordinate of the tile to be changed

	\param newStatus
		The new state of the tile
*/
/*************************************************************************************************/
void MapMatrix::SetTile(int xCoord, int yCoord, TileStatus newStatus)
{
	// Updates the relevant tile
	mapMatrix[xCoord][yCoord] = newStatus;
}

/*************************************************************************************************/
/*!
	\brief
		Moves the player to a specific position. Returns false if the move would be illegal

	\param xCoord
		The x coordinate of the tile to be changed

	\param yCoord
		The y coordinate of the tile to be changed

	\return
		Whether the player was moved successfully
*/
/*************************************************************************************************/
bool MapMatrix::SetPlayerPosition(int xCoord, int yCoord)
{
	// Checks that the coordinates are within the map
	if (xCoord >= 0 && xCoord < mapMatrix.size() && yCoord >= 0 && yCoord < mapMatrix[xCoord].size())
	{
		// Checks that the target location is a valid place to move to
		if (mapMatrix[xCoord][yCoord] < TileStatus::Player)
		{
			// Resets the old player position tile
			mapMatrix[playerPos.first][playerPos.second] = TileStatus::Empty;

			// Sets the new player position
			mapMatrix[xCoord][yCoord] = TileStatus::Player;
			playerPos = std::pair(xCoord, yCoord);

			// Returns that the player was moved
			return true;
		}
	}

	// Otherwise returns that the player wasn't moved
	return false;
}

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
MapMatrix::TileStatus MapMatrix::GetTile(int xCoord, int yCoord)
{
	// Checks that the coordinates are within the map
	if (xCoord >= 0 && xCoord < mapMatrix.size() && yCoord >= 0 && yCoord < mapMatrix[xCoord].size())
	{
		// Returns the tile's status
		return mapMatrix[xCoord][yCoord];
	}

	// Otherwise return a wall
	return TileStatus::Wall;
}

/*************************************************************************************************/
/*!
	\brief
		Returns the player's position

	\return
		The position of the player in the map grid
*/
/*************************************************************************************************/
std::pair<int, int> MapMatrix::GetPlayerPosition()
{
	return playerPos;
}

/*************************************************************************************************/
/*!
	\brief
		Returns that maximum width of the map grid

	\return
		The maximum width of the map grid
*/
/*************************************************************************************************/
int MapMatrix::GetMaxMapWidth()
{
	return (int)mapMatrix.size();
}

/*************************************************************************************************/
/*!
	\brief
		Returns that maximum height of the map grid

	\return
		The maximum height of the map grid
*/
/*************************************************************************************************/
int MapMatrix::GetMaxMapHeight()
{
	return mapMatrix[0].capacity();
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
