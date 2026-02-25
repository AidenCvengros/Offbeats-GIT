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

// Base includes
#include "MapMatrix.h"
#include "../Engine/cppShortcuts.h"

// Includes the input manager for debug inputs
#include "../Engine/InputManager.h"

// Includes window class to be drawn to
#include "../Engine/Window.h"

#include <fstream>

//-------------------------------------------------------------------------------------------------
// Private Constants
//-------------------------------------------------------------------------------------------------

const int bufferSize = 512;					// The default buffer size used for reading files

//-------------------------------------------------------------------------------------------------
// Public Declarations
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Private Variables
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
MapMatrix::MapMatrix(int width, int height) : debugDraw(false), defaultWallTexture(NULL), defaultWallColor(0)
{
	// Creates width number of columns
	for (int i = 0; i < width; i++)
	{
		// Makes the columns height tall
		std::vector<MapTile> column;
		column.resize(height, { TileStatus::Empty, NULL });
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
		Updates the system. Virtual function that must be overwritten by derived class

	\param
		The time elapsed since the previous frame.
*/
/*************************************************************************************************/
void MapMatrix::Update(double dt)
{
	if (_InputManager->CheckInputStatus(InputManager::Inputs::F1) == InputManager::InputStatus::Pressed)
	{
		// Toggles the debug draw
		debugDraw = !debugDraw;
	}
}

/*************************************************************************************************/
/*!
	\brief
		Draws the map. Only used for debug purposes
*/
/*************************************************************************************************/
void MapMatrix::Draw()
{
	// Creates a square game object that's gonna be the drawing
	GameObject wallObject({ 0.0f, 0.0f }, 0.0f, { 2.0f, 2.0f }, 49, true, defaultWallTexture, defaultWallColor);

	// Draws the map
	for (int i = 0; i < mapMatrix.size(); i++)
	{
		for (int j = 0; j < mapMatrix[i].size(); j++)
		{
			// Checks if the tile has a game object
			if (mapMatrix[i][j].tileObject)
			{
				_Window->DrawGameObject(mapMatrix[i][j].tileObject);
			}

			// Otherwise checks if the tile is a blank wall
			if (mapMatrix[i][j].tileStatus == TileStatus::Wall)
			{
				wallObject.SetPosition(ConvertMapCoordsToWorldCoords({ i, j }));
				_Window->DrawGameObject(&wallObject);
			}
		}
	}

	if (debugDraw)
	{
		// Creates a square game object that's gonna be the drawing
		GameObject debugSquare({ 0.0f, 0.0f }, 0.0f, { 2.0f, 2.0f }, 49, true, { 0.0f, 0.0f, 0.0f, 0.3f });
		debugSquare.SetRender(true);

		// Draws the map
		for (int i = 0; i < mapMatrix.size(); i++)
		{
			for (int j = 0; j < mapMatrix[i].size(); j++)
			{
				// Boolean to track whether a square should be drawn on this tile
				bool shouldDraw = false;

				// Checks for the player
				switch (mapMatrix[i][j].tileStatus)
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
					debugSquare.SetPosition({ ConvertMapCoordToWorldCoord(i), ConvertMapCoordToWorldCoord(j) });
					_Window->DrawGameObject(&debugSquare);
				}
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
		mapMatrix[i].resize(mapMatrix.capacity(), { TileStatus::Empty, NULL });
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

	\param gameObject
		The game object associated with the tile. Defaults to null if there's not meant to be a game object
*/
/*************************************************************************************************/
void MapMatrix::SetTile(int xCoord, int yCoord, TileStatus newStatus, GameObject* gameObject)
{
	// Checks if the coordinates are valid
	if (ValidateCoordinates(xCoord, yCoord))
	{
		// Updates the relevant tile
		mapMatrix[xCoord][yCoord] = { newStatus, gameObject };
	}
}

/*************************************************************************************************/
/*!
	\brief
		Moves the player to a specific position. Returns false if the move would be illegal

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
bool MapMatrix::SetPlayerPosition(int xCoord, int yCoord, GameObject* playerObject)
{
	// Checks that the coordinates are within the map
	if (ValidateCoordinates(xCoord, yCoord))
	{
		// Checks that the target location is a valid place to move to
		if (mapMatrix[xCoord][yCoord].tileStatus < TileStatus::Player)
		{
			// Resets the old player position tile
			//mapMatrix[playerPos.first][playerPos.second] = { TileStatus::Empty, NULL };

			// Sets the new player position
			//mapMatrix[xCoord][yCoord] = { TileStatus::Player, playerObject };
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
		Sets the default wall texture that will be used to draw the walls in the current scene

	\param defaultWallTexture_
		The given wall texture

	\param defaultWallColor_
		The color that will be applied to that wall texture
*/
/*************************************************************************************************/
void MapMatrix::SetDefaultWallTexture(Texture* defaultWallTexture_, glm::vec4 defaultWallColor_)
{
	defaultWallTexture = defaultWallTexture_;
	defaultWallColor = defaultWallColor_;
}

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
bool MapMatrix::MoveTile(int prevXCoord, int prevYCoord, int newXCoord, int newYCoord, MapMatrix::TileStatus tileStatus, GameObject* object)
{
	// Checks that the coordinates are within the map
	if (ValidateCoordinates(prevXCoord, prevYCoord) && ValidateCoordinates(newXCoord, newYCoord))
	{
		// Checks that the target location is a valid place to move to
		if (mapMatrix[newXCoord][newYCoord].tileStatus < tileStatus)
		{
			// Resets the old position's tile
			SetTile(prevXCoord, prevYCoord, TileStatus::Empty);

			// Sets the new player position
			SetTile(newXCoord, newYCoord, tileStatus, object);

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
		Sets a given tile to empty and deletes the associated game object

	\param xCoord
		The x coordinate of the tile to be cleared

	\param yCoord
		The y coordinate of the tile to be cleared
*/
/*************************************************************************************************/
void MapMatrix::ClearTile(int xCoord, int yCoord)
{
	// Checks that the tile has a game object
	if (mapMatrix[xCoord][yCoord].tileObject)
	{
		// Destroys the game object
		mapMatrix[xCoord][yCoord].tileObject->SetToBeDestroyed(true);
	}
	
	// Sets the tile to empty
	SetTile(xCoord, yCoord, TileStatus::Empty);
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
MapMatrix::MapTile MapMatrix::GetTile(int xCoord, int yCoord)
{
	// Checks that the coordinates are within the map
	if (ValidateCoordinates(xCoord, yCoord))
	{
		// Returns the tile's status
		return mapMatrix[xCoord][yCoord];
	}

	// Otherwise return a wall
	return { TileStatus::Wall, NULL };
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
	return (int)mapMatrix[0].capacity();
}

/*************************************************************************************************/
/*!
	\brief
		Calculates tile coordinates with a left or right offset

	\param xCoord
		The X coordinate of the center tile

	\param yCoord
		The Y coordinate of the center tile

	\param facingRight
		Whether the attack is facing right

	\param xOffset
		The offset from the center tile. Will be used in conjunction with the facingRight variable to check the correct tile

	\param yOffset
			The vertical offset from the center tile. Defaults to 0 if you're just doing horizontal offsets

	\return
		Returns the tile coordinates
*/
/*************************************************************************************************/
std::pair<int, int> MapMatrix::CalculateOffsetTile(int xCoord, int yCoord, bool facingRight, int xOffset, int yOffset)
{
	// Checks if the attack is facing right
	if (facingRight)
	{
		return std::make_pair(xCoord + xOffset, yCoord + yOffset);
	}
	// Otherwise when the attack is facing left
	else
	{
		return std::make_pair(xCoord - xOffset, yCoord + yOffset);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Reads in a map from the given file

	\param filename
		The file to build the map from

	\param specialTileList
		The list of all nonstandard tiles that need filling
*/
/*************************************************************************************************/
void MapMatrix::ReadMapFromFile(std::string filename, std::vector< std::pair< char, std::pair< int, int > > >& specialTileList)
{
	// Opens the file
	std::ifstream fileInput(filename);
	if (fileInput)
	{
		// Reads in the file
		char bufferInput[bufferSize] = { 0 };
		int mapWidth;
		int mapHeight;

		// Reads in the width and height of the map, then moves to the next line
		fileInput >> mapWidth;
		fileInput.getline(bufferInput, bufferSize, ',');
		fileInput >> mapHeight;
		fileInput.getline(bufferInput, bufferSize);

		// Sets the size of the map matrix
		mapMatrix.clear();
		for (size_t i = 0; i < mapWidth; i++)
		{
			// Makes the columns height tall
			std::vector<MapTile> column;
			column.resize(mapHeight, { TileStatus::Empty, NULL });
			mapMatrix.push_back(column);
		}

		// Fills in the map
		for (int i = mapHeight - 1; i >= 0; i--)
		{
			fileInput.getline(bufferInput, bufferSize);

			for (int j = 0; j < mapWidth; j++)
			{
				// Checks what the next character is
				if (bufferInput[j * 2] == 'w')
				{
					SetTile(j, i, TileStatus::Wall);
				}
				else if (bufferInput[j * 2] == 'p')
				{
					SetTile(j, i, TileStatus::Player);
					playerPos = { j, i };
				}
				else if (bufferInput[j * 2] != '0')
				{
					// If the space isn't empty or something simple, notes it so the scene can populate these objects
					specialTileList.push_back({ bufferInput[j * 2], {j, i} });
				}
			}
		}
	}
	// If the file didn't read, throw an error
	else
	{
		throw std::runtime_error("Failed to read in scene map");
	}
}

/*************************************************************************************************/
/*!
	\brief
		Updates the player's position to the map's version.

	\param playerObject
		The player game object
*/
/*************************************************************************************************/
void MapMatrix::UpdatePlayerPosition(GameObject* playerObject)
{
	playerObject->SetMapCoords(playerPos);
	UpdateObjectPosition(playerPos, playerObject);
}

/*************************************************************************************************/
/*!
	\brief
		Updates an object's visual position based on their map position. Will center the object on the tile.

	\param xCoord
		The x coordinate of the tile to be changed

	\param yCoord
		The y coordinate of the tile to be changed

	\param playerObject
		The player game object
*/
/*************************************************************************************************/
void MapMatrix::UpdateObjectPosition(int xCoord, int yCoord, GameObject* object)
{
	object->SetPosition({ xCoord * 2.0f, yCoord * 2.0f });
}

/*************************************************************************************************/
/*!
	\brief
		Returns whether the given tile status is a sticker

	\param tileStatus
		The given tile status

	\return
		Whether the tile is a sticker
*/
/*************************************************************************************************/
bool MapMatrix::IsSticker(TileStatus tileStatus)
{
	// Checks for the relevant tile statuses
	if (tileStatus == TileStatus::Sticker ||
		tileStatus == TileStatus::Bumper ||
		tileStatus == TileStatus::SandBlock)
	{
		return true;
	}

	// Otherwise returns false
	return false;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

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
bool MapMatrix::ValidateCoordinates(int xCoord, int yCoord)
{
	return xCoord >= 0 && xCoord < mapMatrix.size() && yCoord >= 0 && yCoord < mapMatrix[xCoord].size();
}
