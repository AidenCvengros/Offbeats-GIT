/*************************************************************************************************/
/*!
\file Player.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.2.16
\brief
    The game object representing the player

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Player.h"
#include "../Engine/cppShortcuts.h"

// The inventory class
#include "../Gameplay/Inventory.h"

// The input manager class so the player can use inputs
#include "../Engine/InputManager.h"

// The map matrix class so the player can exist within the map
#include "../Gameplay/MapMatrix.h"

// Other game object classes so the player can interact with them
#include "Enemy.h"
#include "LockedWall.h"
#include "Item.h"
#include "Key.h"

// std::clamp is used to clamp the player's speed
#include <algorithm>

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
		Constructor for the player game object class

	\param scene
		The scene that this game object is in

	\param pos
		The position of the game object

	\param rot
		The rotation of the game object

	\param sca
		The scale of the game object

	\param drawPriority_
		Higher draw priorities are drawn in front of objects with lower priority

	\param texture_
		The texture of the player object

	\param mapCoords
		The starting coordinates of the player
*/
/*************************************************************************************************/
Player::Player(glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, Texture* texture_, std::pair<int, int> mapCoords) :
	GameObject(pos, rot, sca, drawPriority_, true, texture_, { 1.0f, 1.0f, 1.0f, 1.0f }, mapCoords),
	timeSinceMove(0.0), horizontalVelocity(0.0f), verticalVelocity(0.0f), grounded(true),
	lowerInnerGap(sca.x * 0.0625f), upperInnerGap(sca.x * 0.0625f), actionManager(), inventory(NULL)
{
	_MapMatrix->SetPlayerPosition(mapCoords, this);
	inventory = new Inventory();
}

/*************************************************************************************************/
/*!
	\brief
		Destructor for the player class
*/
/*************************************************************************************************/
Player::~Player()
{
	delete inventory;
}

/*************************************************************************************************/
/*!
	\brief
		Updates the game object.

	\param dt
		The time elapsed since the previous frame
*/
/*************************************************************************************************/
void Player::Update(double dt)
{
	//// Updates movements if any are active
	//if (GetMoving())
	//{
	//	MoveToUpdate(dt);
	//}

	// Checks if the player is trying to move left
	if (CheckInput(InputManager::Inputs::Left) && !CheckInput(InputManager::Inputs::Right))
	{
		// Accelerates normally on the ground
		if (grounded)
		{
			// Accelerates the player to the left
			AcceleratePlayerHorizontal(-24.0f, dt);
		}
		else
		{
			// Accelerates the player to the left more slowly in the air
			AcceleratePlayerHorizontal(-8.0f, dt);
		}
	}
	// Checks if the player is trying to move right
	else if (CheckInput(InputManager::Inputs::Right) && !CheckInput(InputManager::Inputs::Left))
	{
		// Accelerates normally on the ground
		if (grounded)
		{
			// Accelerates the player to the right
			AcceleratePlayerHorizontal(24.0f, dt);
		}
		else
		{
			// Accelerates the player to the right more slowly in the air
			AcceleratePlayerHorizontal(8.0f, dt);
		}
	}
	// Otherwise doesn't accelerate the player
	else
	{
		AcceleratePlayerHorizontal(0.0f, dt);
	}

	// Checks if the player has left the ground
	if (UngroundedCheck())
	{
		AcceleratePlayerVertical(-70.0f, dt);
	}

	// Checks for the jump input
	if (CheckInput(InputManager::Inputs::Jump))
	{
		// Checks if the player is grounded
		if (grounded)
		{
			// Jumps
			AcceleratePlayerVertical(30.0f, 1.0f);
			grounded = false;
		}
	}

	// Moves the player
	MovePlayer(dt);
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Returns whether the given input was either pressed or held

	\param input
		The given input

	\return
		Whether the input was pressed or held
*/
/*************************************************************************************************/
bool Player::CheckInput(InputManager::Inputs input)
{
	return _InputManager->CheckInputStatus(input) == InputManager::InputStatus::Pressed
		|| _InputManager->CheckInputStatus(input) == InputManager::InputStatus::Held;
}

/*************************************************************************************************/
/*!
	\brief
		Helper function to accelerate the player horizontally

	\param accelerationAmount
		How much the player should accelerate by

	\param dt
		The time elapsed since the previous frame
*/
/*************************************************************************************************/
void Player::AcceleratePlayerHorizontal(float accelerationAmount, double dt)
{
	// If we are accelerating
	if (horizontalVelocity * accelerationAmount > 0.0f)
	{
		// Accelerates faster at first
		if (abs(horizontalVelocity) <= 8.0)
		{
			// Adjusts the horizontal velocity of the player
			horizontalVelocity += accelerationAmount * (float)dt;
		}
		// But maxxing out speed takes a little longer
		else
		{
			horizontalVelocity += accelerationAmount * (float)dt / 4.0f;
		}
	}
	// If we are starting from a standstill, gets an extra boost
	else if (horizontalVelocity == 0.0f)
	{
		horizontalVelocity += accelerationAmount * 0.1;
	}
	// If we are stopping
	else
	{
		horizontalVelocity += accelerationAmount * (float)dt * 1.5f;
	}

	// Horizontal velocity deteriorates over time if not being added
	if (accelerationAmount == 0.0f)
	{
		// If we've basically stopped, just stops
		if (abs(horizontalVelocity) < 0.25f)
		{
			horizontalVelocity = 0.0f;
		}
		// Otherwise, slows down
		else
		{
			horizontalVelocity /= 1.0f + (1.0f * (float)dt);
		}
	}

	// Puts a max cap to the player's velocity
	horizontalVelocity = std::clamp(horizontalVelocity, -20.0f, 20.0f);
}

/*************************************************************************************************/
/*!
	\brief
		Helper function to accelerate the player vertically

	\param accelerationAmount
		How much the player should accelerate by

	\param dt
		The time elapsed since the previous frame
*/
/*************************************************************************************************/
void Player::AcceleratePlayerVertical(float accelerationAmount, double dt)
{
	// Applies the acceleration amount to the vertical velocity
	verticalVelocity += accelerationAmount * dt;

	// Clamps the player's max speed
	verticalVelocity = std::clamp(verticalVelocity, -30.0f, 30.0f);
}

/*************************************************************************************************/
/*!
	\brief
		Helper function to manage moving the player

	\param dt
		The time elapsed since the previous frame
*/
/*************************************************************************************************/
void Player::MovePlayer(double dt)
{
	// Gets the player's position
	glm::vec2 playerWorldPosition = GetPosition();

	// Calculates how far we are moving this frame
	float horizontalMovement = horizontalVelocity * (float)dt;
	float verticalMovement = verticalVelocity * (float)dt;

	// Tries to move the player horizontally
	playerWorldPosition.x += horizontalMovement;

	// Updates whether we're facing right or left
	if (horizontalMovement > 0.0f)
	{
		SetIsFacingRight(true);

		// Checks if the right side of the player has moved into an object
		std::pair<int, int> rightBottomSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::BottomRightOut);
		std::pair<int, int> rightTopSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::TopRightIn);
		if (_MapMatrix->GetTile(rightBottomSideTile).tileStatus > MapMatrix::TileStatus::Player ||
			_MapMatrix->GetTile(rightTopSideTile).tileStatus > MapMatrix::TileStatus::Player)
		{
			playerWorldPosition.x = ConvertMapCoordsToWorldCoords(rightBottomSideTile).x - 2.015625f;

			// Kills the player's velocity
			horizontalVelocity = 0;
		}
	}
	else if (horizontalMovement < 0.0f)
	{
		SetIsFacingRight(false);

		// Checks if the left side of the player has moved into an object
		std::pair<int, int> leftBottomSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::BottomLeftOut);
		std::pair<int, int> leftTopSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::TopLeftIn);
		if (_MapMatrix->GetTile(leftBottomSideTile).tileStatus > MapMatrix::TileStatus::Player ||
			_MapMatrix->GetTile(leftTopSideTile).tileStatus > MapMatrix::TileStatus::Player)
		{
			playerWorldPosition.x = ConvertMapCoordsToWorldCoords(leftBottomSideTile).x + 2.015625f;

			// Kills the player's velocity
			horizontalVelocity = 0;
		}
	}

	// Tries to move the player vertically 
	playerWorldPosition.y += verticalMovement;

	// Checks if the player is moving up or down
	if (verticalMovement > 0.0f)
	{
		// Checks if the right side of the player has moved into an object
		std::pair<int, int> topLeftSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::TopLeftIn);
		std::pair<int, int> topRightSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::TopRightIn);
		if (_MapMatrix->GetTile(topLeftSideTile).tileStatus > MapMatrix::TileStatus::Player ||
			_MapMatrix->GetTile(topRightSideTile).tileStatus > MapMatrix::TileStatus::Player)
		{
			playerWorldPosition.y = ConvertMapCoordsToWorldCoords(topLeftSideTile).y - 2.015625f;

			// Kills the player's velocity
			verticalVelocity = 0;
		}
	}
	else if (verticalMovement < 0.0f)
	{
		// Checks if the right side of the player has moved into an object
		std::pair<int, int> bottomLeftSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::BottomLeftOut);
		std::pair<int, int> bottomRightSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::BottomRightOut);
		if (_MapMatrix->GetTile(bottomLeftSideTile).tileStatus > MapMatrix::TileStatus::Player ||
			_MapMatrix->GetTile(bottomRightSideTile).tileStatus > MapMatrix::TileStatus::Player)
		{
			playerWorldPosition.y = ConvertMapCoordsToWorldCoords(bottomLeftSideTile).y + 2.0;// 15625f;

			grounded = true;

			// Kills the player's velocity
			verticalVelocity = 0;
		}
	}

	//// Runs collision checks
	//std::pair<bool, bool> collisionChecks = CollisionCheck(horizontalMovement, verticalMovement);
	//
	//// If we're not at risk of hitting anything horizontally
	//if (collisionChecks.first == false)
	//{
	//	// Just moves the player
	//	playerWorldPosition.x += horizontalMovement;
	//}
	//// If we can hit something horizontally
	//else
	//{
	//	// Calculates the distance between the player and the block next to them
	//	float nextTileWorldXPosition = ConvertMapCoordsToWorldCoords(_MapMatrix->CalculateOffsetTile(playerMapPosition, GetIsFacingRight(), 1)).x;
	//	float distFromObject = nextTileWorldXPosition - GetPosition().x;
	//
	//	// Checks if the amount we move this frame would put us in that object
	//	if (1.0f + horizontalMovement > abs(distFromObject))
	//	{
	//		// If so, moves us up against the wall
	//		if (distFromObject > 0)
	//		{
	//			playerWorldPosition.x = nextTileWorldXPosition - (1.0f + halfWidth);
	//		}
	//		else if (distFromObject < 0)
	//		{
	//			playerWorldPosition.x = nextTileWorldXPosition + (1.0f + halfWidth);
	//		}
	//
	//		// Kills the player's velocity
	//		horizontalVelocity = 0;
	//	}
	//	// If we aren't going to collide, moves the player
	//	else
	//	{
	//		playerWorldPosition.x += horizontalMovement;
	//	}
	//}
	//
	//// If we're not at risk of hitting anything vertically
	//if (collisionChecks.second == false)
	//{
	//	// Just moves the player
	//	playerWorldPosition.y += verticalMovement;
	//}
	//// If we can hit something vertically
	//else
	//{
	//	// Determines if we are going up or down
	//	int directionModifier = 1;
	//	if (verticalMovement < 0)
	//	{
	//		directionModifier = -1;
	//	}
	//
	//	// Calculates the distance between the player and the block next to them
	//	float nextTileWorldYPosition = ConvertMapCoordsToWorldCoords(_MapMatrix->CalculateOffsetTile(playerMapPosition, GetIsFacingRight(), 0, directionModifier)).y;
	//	float distFromObject = nextTileWorldYPosition - GetPosition().y;
	//
	//	// Checks if the amount we move this frame would put us in that object
	//	if (1.0f + halfHeight + abs(verticalMovement) > abs(distFromObject))
	//	{
	//		// If so, moves us up against the wall
	//		if (distFromObject > 0)
	//		{
	//			playerWorldPosition.y = nextTileWorldYPosition - (1.0f + halfHeight);
	//		}
	//		else if (distFromObject < 0)
	//		{
	//			playerWorldPosition.y = nextTileWorldYPosition + (1.0f + halfHeight);
	//
	//			// If we moved onto something below us, marks us as grounded
	//			grounded = true;
	//		}
	//
	//		// Kills the player's velocity
	//		verticalVelocity = 0;
	//	}
	//	// If we aren't going to collide, moves the player
	//	else
	//	{
	//		playerWorldPosition.y += verticalMovement;
	//	}
	//}

	// Sets the player's new position
	SetPosition(playerWorldPosition);

	// Updates the player coordinates now that we've moved
	UpdatePlayerCoords();
}

/*************************************************************************************************/
/*!
	\brief
		Helper function to check if the player is going to run into anything this turn

	\param horizontalMovement
		How far the player plans to move horizontally this frame

	\param verticalMovement
		How far the player plans to move vertically this frame

	\return
		Returns a pair of booleans. The first one says if we hit horizontally, the second says if we hit vertically
*/
/*************************************************************************************************/
std::pair<bool, bool> Player::CollisionCheck(float horizontalMovement, float verticalMovement)
{
	// Variables to track collision
	bool horizontalCollision = false;
	bool verticalCollsion = false;

	// Gets the player position
	std::pair<int, int> playerPosition = _MapMatrix->GetPlayerPosition();

	// Checks if there even is anything to collide with in the space to the side
	if (_MapMatrix->GetTile(_MapMatrix->CalculateOffsetTile(playerPosition, GetIsFacingRight(), 1)).tileStatus > MapMatrix::TileStatus::Player)
	{
		horizontalCollision = true;
	}
	
	// Checks if there even is anything to collide with in the space vertical
	int verticalOffset = 0;
	if (verticalMovement > 0.0f)
	{
		verticalOffset = 1;
	}
	else if (verticalMovement < 0.0f)
	{
		verticalOffset = -1;
	}
	if (_MapMatrix->GetTile(_MapMatrix->CalculateOffsetTile(CalculatePlayerMapPositions(GetPosition(), Positions::BottomLeftIn), GetIsFacingRight(), 0, verticalOffset)).tileStatus > MapMatrix::TileStatus::Player ||
		_MapMatrix->GetTile(_MapMatrix->CalculateOffsetTile(CalculatePlayerMapPositions(GetPosition(), Positions::BottomRightIn), GetIsFacingRight(), 0, verticalOffset)).tileStatus > MapMatrix::TileStatus::Player)
	{
		verticalCollsion = true;
	}

	// Checks if we have a case where we're coming directly at a corner
	if (horizontalCollision == false && verticalCollsion == false && _MapMatrix->GetTile(_MapMatrix->CalculateOffsetTile(_MapMatrix->GetPlayerPosition(), GetIsFacingRight(), 1, verticalOffset)).tileStatus > MapMatrix::TileStatus::Player)
	{
		// We turn on the horizontal collision so the player effectively hits the wall
		horizontalCollision = true;
	}

	// Returns the collision results
	return { horizontalCollision, verticalCollsion };
}

/*************************************************************************************************/
/*!
	\brief
		Helper function to check if the player has left the ground (usually by walking off a ledge)

	\return
		Whether the player left the ground
*/
/*************************************************************************************************/
bool Player::UngroundedCheck()
{
	// Checks if the player was grounded
	if (grounded)
	{
		// Checks if there is now nothing to collide with in the space under the player
		if (_MapMatrix->GetTile(_MapMatrix->CalculateOffsetTile(_MapMatrix->GetPlayerPosition(), GetIsFacingRight(), 0, -1)).tileStatus < MapMatrix::TileStatus::Player &&
			_MapMatrix->GetTile(_MapMatrix->CalculateOffsetTile(CalculatePlayerMapPositions(GetPosition(), Positions::BottomRightOut), GetIsFacingRight(), 0, -1)).tileStatus < MapMatrix::TileStatus::Player)
		{
			// Marks the player as ungrounded
			grounded = false;
		}
		else
		{
			// If the player is still grounded, returns false
			return false;
		}
	}
	
	// If the player isn't grounded, returns true
	return true;
}

/*************************************************************************************************/
/*!
	\brief
		Helper function to update the coordinates of what square the player is in
*/
/*************************************************************************************************/
void Player::UpdatePlayerCoords()
{
	std::pair<int, int> playerCoords = ConvertWorldCoordsToMapCoords(GetPosition().x, GetPosition().y);

	if (playerCoords != _MapMatrix->GetPlayerPosition())
	{
		_MapMatrix->SetPlayerPosition(playerCoords, this);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Helper function to manage moving the player

	\param dt
		The time elapsed since the previous frame

	\param playerPosition
		The current position of the player (will be modified if the player moves)
*/
/*************************************************************************************************/
void Player::Interact(double dt, std::pair<int, int>& playerPosition)
{
	// Checks if the player is starting an action
	//if (actionQueued == PlayerActions::INTERACT)
	//{
	//	// If the player isn't moving, they can start the action
	//	if (GetIsMoving() == false)
	//	{
	//		std::pair<int, int> interactionCoords = _MapMatrix->CalculateOffsetTile(GetMapCoords(), GetIsFacingRight(), 1);
	//		InteractWithTile(interactionCoords, true, true);
	//	}
	//}
}

/*************************************************************************************************/
/*!
	\brief
		Helper function to manage interacting with tiles

	\param targetTileCoords
		The tile being interacted with

	\param destructibles
		Whether this interaction should destroy objects

	\param collectibles
		Whether this interaction should collect objects
*/
/*************************************************************************************************/
void Player::InteractWithTile(std::pair<int, int> targetTileCoords, bool destructibles, bool collectibles)
{
	// Gets the tile
	MapMatrix::MapTile targetTile = _MapMatrix->GetTile(targetTileCoords);

	// Destroys destructible walls
	if (destructibles)
	{
		if (targetTile.tileStatus == MapMatrix::TileStatus::Destructible)
		{
			_MapMatrix->ClearTile(targetTileCoords.first, targetTileCoords.second);
		}
	}

	// Grabs collectibles and adds it to inventory
	if (collectibles)
	{
		// Checks for key
		if (targetTile.tileStatus == MapMatrix::TileStatus::Key && targetTile.tileObject && ((Item*)targetTile.tileObject)->GetItemType() == Item::ItemType::Key)
		{
			if (inventory->AddKey((Key*)targetTile.tileObject))
			{
				_MapMatrix->ClearTile(targetTileCoords.first, targetTileCoords.second);
			}
		}
		// Checks for coin
		if (targetTile.tileStatus == MapMatrix::TileStatus::Coin)
		{
			inventory->AddCoin();
			_MapMatrix->ClearTile(targetTileCoords.first, targetTileCoords.second);
		}
	}

	// Opens doors
	if (targetTile.tileStatus == MapMatrix::TileStatus::LockedDoor && targetTile.tileObject)
	{
		if (inventory->HaveKey(((LockedWall*)targetTile.tileObject)->GetKeyValue()))
		{
			_MapMatrix->ClearTile(targetTileCoords.first, targetTileCoords.second);
		}
	}
}

/*************************************************************************************************/
/*!
	\brief
		Calculates the map tile of the player

	\param position
		The given world position to calculate map positions for (can be arbitrary which is useful for collision checks before we've moved the player)

	\param position
		The enum saying what position to return

	\return
		The calculated map tile
*/
/*************************************************************************************************/
std::pair<int, int> Player::CalculatePlayerMapPositions(glm::vec2 position, Player::Positions anchorPoint)
{
	switch (anchorPoint)
	{
	case Player::Positions::Center:
		return ConvertWorldCoordsToMapCoords(position + (GetScale() * 0.5f));
		break;
	case Player::Positions::BottomLeftOut:
		return ConvertWorldCoordsToMapCoords(position);
		break;
	case Player::Positions::BottomLeftIn:
		return ConvertWorldCoordsToMapCoords(position.x, position.y + lowerInnerGap);
		break;
	case Player::Positions::BottomRightIn:
		return ConvertWorldCoordsToMapCoords(position.x + GetScale().x, position.y + lowerInnerGap);
		break;
	case Player::Positions::BottomRightOut:
		return ConvertWorldCoordsToMapCoords(position.x + GetScale().x, position.y);
		break;
	case Player::Positions::TopLeftOut:
		return ConvertWorldCoordsToMapCoords(position.x, position.y + GetScale().y);
		break;
	case Player::Positions::TopLeftIn:
		return ConvertWorldCoordsToMapCoords(position.x, position.y + GetScale().y - upperInnerGap);
		break;
	case Player::Positions::TopRightIn:
		return ConvertWorldCoordsToMapCoords(position.x + GetScale().x, position.y + GetScale().y - upperInnerGap);
		break;
	case Player::Positions::TopRightOut:
		return ConvertWorldCoordsToMapCoords(position.x + GetScale().x, position.y + GetScale().y);
		break;
	default:
		// If something went wrong, returns (-1, -1)
		return std::make_pair(-1, -1);
		break;
	}
}
