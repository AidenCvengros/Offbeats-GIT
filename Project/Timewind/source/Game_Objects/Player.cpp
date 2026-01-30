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
#include "Key.h"
#include "Stickers/Bumper.h"
#include "../Engine/Window.h"
#include "Camera.h"

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
	timeSinceMove(0.0), horizontalVelocity(0.0f), verticalVelocity(0.0f), grounded(true), jumped(false), againstWall(0), goingMaxSpeed(false), maxSpeed(20.0f), currentPlayerState(PlayerStates::Running),
	lowerInnerGap(sca.x * 0.0625f), upperInnerGap(sca.x * 0.125f), actionManager(), inventory(NULL)
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
	// Flips the player state
	if (_InputManager->CheckInputStatus(InputManager::Inputs::Swap) == InputManager::InputStatus::Pressed)
	{
		// If running set to placing
		if (currentPlayerState == PlayerStates::Running || currentPlayerState == PlayerStates::Walking)
		{
			currentPlayerState = PlayerStates::Placing;
		}
		// If placing set to walking
		else if (currentPlayerState == PlayerStates::Placing)
		{
			currentPlayerState = PlayerStates::Running;
		}
	}

	// If the player is running
	if (currentPlayerState == PlayerStates::Running)
	{
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
			AcceleratePlayerVertical(-65.0f, dt);
		}

		// Checks for the jump input
		if (jumped == false && CheckInput(InputManager::Inputs::Jump))
		{
			// Checks if the player is grounded or cut in by a roof
			if (grounded &&
				_MapMatrix->GetTile(_MapMatrix->CalculateOffsetTile(CalculatePlayerMapPositions(GetPosition(), Positions::TopLeftIn), GetIsFacingRight(), 0, 1)).tileStatus < MapMatrix::TileStatus::Player &&
				_MapMatrix->GetTile(_MapMatrix->CalculateOffsetTile(CalculatePlayerMapPositions(GetPosition(), Positions::TopRightIn), GetIsFacingRight(), 0, 1)).tileStatus < MapMatrix::TileStatus::Player)
			{
				// Jumps higher if the player is at max speed
				if (goingMaxSpeed)
				{
					AcceleratePlayerVertical(37.0f, 1.0f);
				}
				else
				{
					AcceleratePlayerVertical(30.0f, 1.0f);
				}

				// No longer grounded
				grounded = false;
				jumped = true;
			}
		}
		// Checks if the jump input was released
		else if (_InputManager->CheckInputStatus(InputManager::Inputs::Jump) == InputManager::InputStatus::Released)
		{
			if (verticalVelocity >= 15.0f)
			{
				// If the jump was let go, cuts the jump short
				verticalVelocity *= 0.5f;
			}

			// Resets the jumped boolean
			jumped = false;
		}

		// Moves the player
		MovePlayer(dt);

		// Checks if the player pressed the interaction button
		if (CheckInput(InputManager::Inputs::Action))
		{
			InteractWithTile(_MapMatrix->CalculateOffsetTile(_MapMatrix->GetPlayerPosition(), GetIsFacingRight(), 1, 0), false, false);
		}
	}

	// If the player is placing
	else if (currentPlayerState == PlayerStates::Placing)
	{
		// Checks if there is a bumper in inventory
		Bumper* bumper = (Bumper*)inventory->GetSelectedSticker(0);
		if (bumper)
		{
			// When the player presses the button
			if (CheckInput(InputManager::Inputs::Action))
			{
				// If the space the camera is looking at is empty
				std::pair<int, int> cursorTile = ConvertWorldCoordsToMapCoords(_Window->GetCamera()->GetLookAtPosition());
				if (_MapMatrix->GetTile(cursorTile).tileStatus == MapMatrix::TileStatus::Empty)
				{
					// Puts the bumper in the empty space
					_MapMatrix->SetTile(cursorTile, MapMatrix::TileStatus::Bumper, bumper);
					bumper->SetStickerActive(true);
					bumper->SetPosition(ConvertMapCoordsToWorldCoords(cursorTile));
					bumper->SetRender(true);
					inventory->ClearSelectedSticker(0);
				}
			}
		}
	}
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
		horizontalVelocity += accelerationAmount * 0.1f;
	}
	// If we are stopping
	else
	{
		horizontalVelocity += accelerationAmount * (float)dt * 2.0f;
	}

	// Horizontal velocity deteriorates over time if not being added
	if (accelerationAmount == 0.0f)
	{
		// If we've basically stopped, just stops
		if (abs(horizontalVelocity) < 4.0f)
		{
			horizontalVelocity = 0.0f;
		}
		// Otherwise, slows down
		else
		{
			horizontalVelocity /= 1.0f + (1.0f * (float)dt);
		}
	}

	// Checks if the player is at max speed
	if (abs(horizontalVelocity) > maxSpeed)
	{
		// Sets that we are going max speed
		goingMaxSpeed = true;
		SetColor({ 1.0f, 0.8f, 0.8f, 1.0f });
	}
	else
	{
		// Otherwise resets max speed
		goingMaxSpeed = false;
		SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	// Puts a max cap to the player's velocity
	horizontalVelocity = std::clamp(horizontalVelocity, -maxSpeed, maxSpeed);
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
	verticalVelocity += accelerationAmount * (float)dt;

	// Clamps the player's max speed
	verticalVelocity = std::clamp(verticalVelocity, -30.0f, 37.0f);
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
	if (!againstWall || horizontalMovement * againstWall > 0.0f)
	{
		playerWorldPosition.x += horizontalMovement;
	}

	// Resets against wall
	againstWall = 0;

	// Updates whether we're facing right or left
	if (horizontalMovement > 0.0f)
	{
		SetIsFacingRight(true);

		// Checks if the right side of the player has moved into an object
		std::pair<int, int> rightBottomSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::BottomRightIn);
		std::pair<int, int> rightTopSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::TopRightIn);
		if (_MapMatrix->GetTile(rightBottomSideTile).tileStatus > MapMatrix::TileStatus::Player ||
			_MapMatrix->GetTile(rightTopSideTile).tileStatus > MapMatrix::TileStatus::Player)
		{
			// Checks if we can move into a side wall
			if (!(verticalMovement > 0.0f && _MapMatrix->GetTile(rightBottomSideTile).tileStatus < MapMatrix::TileStatus::Player &&
				_MapMatrix->GetTile(CalculatePlayerMapPositions({ playerWorldPosition.x, playerWorldPosition.y + verticalMovement }, Positions::TopRightIn)).tileStatus < MapMatrix::TileStatus::Player) &&
				!(verticalMovement > 0.0f && _MapMatrix->GetTile(rightTopSideTile).tileStatus < MapMatrix::TileStatus::Player &&
				_MapMatrix->GetTile(CalculatePlayerMapPositions({ playerWorldPosition.x, playerWorldPosition.y + verticalMovement }, Positions::BottomRightIn)).tileStatus < MapMatrix::TileStatus::Player))
			{
				// If none of that is happening, we are up against a wall
				playerWorldPosition.x = ConvertMapCoordsToWorldCoords(rightBottomSideTile).x - 2.0078125f + upperInnerGap;

				// Kills the player's velocity
				horizontalVelocity = 0;
				againstWall = 1;
			}
		}
	}
	else if (horizontalMovement < 0.0f)
	{
		SetIsFacingRight(false);

		// Checks if the left side of the player has moved into an object
		std::pair<int, int> leftBottomSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::BottomLeftIn);
		std::pair<int, int> leftTopSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::TopLeftIn);
		if (_MapMatrix->GetTile(leftBottomSideTile).tileStatus > MapMatrix::TileStatus::Player ||
			_MapMatrix->GetTile(leftTopSideTile).tileStatus > MapMatrix::TileStatus::Player)
		{
			// Checks if we can move into a side wall
			if (!(verticalMovement < 0.0f && _MapMatrix->GetTile(leftBottomSideTile).tileStatus < MapMatrix::TileStatus::Player &&
				_MapMatrix->GetTile(CalculatePlayerMapPositions({ playerWorldPosition.x, playerWorldPosition.y + verticalMovement }, Positions::TopLeftIn)).tileStatus < MapMatrix::TileStatus::Player) &&
				!(verticalMovement > 0.0f && _MapMatrix->GetTile(leftTopSideTile).tileStatus < MapMatrix::TileStatus::Player &&
				_MapMatrix->GetTile(CalculatePlayerMapPositions({ playerWorldPosition.x, playerWorldPosition.y + verticalMovement }, Positions::BottomLeftIn)).tileStatus < MapMatrix::TileStatus::Player))
			{
				playerWorldPosition.x = ConvertMapCoordsToWorldCoords(leftBottomSideTile).x + 2.0078125f - upperInnerGap;

				// Kills the player's velocity
				horizontalVelocity = 0;
				againstWall = -1;
			}
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
		std::pair<int, int> topCenter = CalculatePlayerMapPositions(playerWorldPosition, Positions::TopCenter);
		if (_MapMatrix->GetTile(topLeftSideTile).tileStatus > MapMatrix::TileStatus::Player ||
			_MapMatrix->GetTile(topRightSideTile).tileStatus > MapMatrix::TileStatus::Player)
		{
			// Checks if the center is clear and if so moves the player towards the opening
			if (_MapMatrix->GetTile(topCenter).tileStatus < MapMatrix::TileStatus::Player)
			{
				float downYPos = ConvertMapCoordsToWorldCoords(topLeftSideTile).y - 2.0f + upperInnerGap;
				float distInObject = playerWorldPosition.y - downYPos;
				playerWorldPosition.y = downYPos;
				
				if (_MapMatrix->GetTile(topLeftSideTile).tileStatus > MapMatrix::TileStatus::Player)
				{
					playerWorldPosition.x += distInObject;
				}
				else
				{
					playerWorldPosition.x -= distInObject;
				}
			}
			// Otherwise bumps into the ceiling
			else
			{
				playerWorldPosition.y = ConvertMapCoordsToWorldCoords(topLeftSideTile).y - 2.0078125f + upperInnerGap;

				// Kills the player's velocity
				verticalVelocity *= 0.25f;

				// If jumping into a destructible object, destroy it
				InteractWithTile(_MapMatrix->CalculateOffsetTile(CalculatePlayerMapPositions(playerWorldPosition, Positions::TopRightIn), GetIsFacingRight(), 0, 1), true, false);
				InteractWithTile(_MapMatrix->CalculateOffsetTile(CalculatePlayerMapPositions(playerWorldPosition, Positions::TopLeftIn), GetIsFacingRight(), 0, 1), true, false);
			}
		}
	}
	else if (verticalMovement < 0.0f)
	{
		// Checks if the right side of the player has moved into an object
		std::pair<int, int> bottomLeftSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::BottomLeftIn);
		std::pair<int, int> bottomRightSideTile = CalculatePlayerMapPositions(playerWorldPosition, Positions::BottomRightIn);
		if (_MapMatrix->GetTile(bottomLeftSideTile).tileStatus > MapMatrix::TileStatus::Player ||
			_MapMatrix->GetTile(bottomRightSideTile).tileStatus > MapMatrix::TileStatus::Player)
		{
			playerWorldPosition.y = ConvertMapCoordsToWorldCoords(bottomLeftSideTile).y + 2.0f;// 15625f;

			grounded = true;

			// Kills the player's velocity
			verticalVelocity = 0;
		}
	}

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
		if (_MapMatrix->GetTile(_MapMatrix->CalculateOffsetTile(CalculatePlayerMapPositions(GetPosition(), Positions::BottomLeftOut), GetIsFacingRight(), 0, -1)).tileStatus < MapMatrix::TileStatus::Player &&
			_MapMatrix->GetTile(_MapMatrix->CalculateOffsetTile(CalculatePlayerMapPositions(GetPosition(), Positions::BottomRightOut), GetIsFacingRight(), 0, -1)).tileStatus < MapMatrix::TileStatus::Player)
		{
			// If the player is going max speed, they can skip over 1 block gaps
			if (goingMaxSpeed && _MapMatrix->GetTile(_MapMatrix->CalculateOffsetTile(CalculatePlayerMapPositions(GetPosition(), Positions::Center), GetIsFacingRight(), 1, -1)).tileStatus > MapMatrix::TileStatus::Player)
			{
				return false;
			}
			else
			{
				// Marks the player as ungrounded
				grounded = false;
			}
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
	// Gets the player's position and map coordinates
	glm::vec2 playerPos = GetPosition();
	std::pair<int, int> playerCoords = CalculatePlayerMapPositions(playerPos, Positions::Center);

	if (playerCoords != _MapMatrix->GetPlayerPosition())
	{
		InteractWithTile(playerCoords, false, true);
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
		else if (targetTile.tileStatus == MapMatrix::TileStatus::Coin)
		{
			inventory->AddCoin();
			_MapMatrix->ClearTile(targetTileCoords.first, targetTileCoords.second);
		}
		// Checks for a sticker
		else if (targetTile.tileStatus == MapMatrix::TileStatus::Sticker)
		{
			inventory->AddSticker((Sticker*)targetTile.tileObject);
			_MapMatrix->SetTile(targetTileCoords, MapMatrix::TileStatus::Empty);
			targetTile.tileObject->SetRender(false);
		}
		// Checks for bumper
		else if (targetTile.tileStatus == MapMatrix::TileStatus::Bumper)
		{
			float bumperStrength = ((Bumper*)targetTile.tileObject)->GetBumperStrength();
			verticalVelocity = cosf(glm::radians(targetTile.tileObject->GetRotation())) * bumperStrength;
			horizontalVelocity = sinf(glm::radians(targetTile.tileObject->GetRotation())) * bumperStrength;
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
		return ConvertWorldCoordsToMapCoords(position.x + lowerInnerGap, position.y);
		break;
	case Player::Positions::BottomLeftIn:
		return ConvertWorldCoordsToMapCoords(position.x + upperInnerGap, position.y);
		break;
	case Player::Positions::BottomRightIn:
		return ConvertWorldCoordsToMapCoords(position.x + GetScale().x - upperInnerGap, position.y);
		break;
	case Player::Positions::BottomRightOut:
		return ConvertWorldCoordsToMapCoords(position.x + GetScale().x - lowerInnerGap, position.y);
		break;
	case Player::Positions::TopLeftOut:
		return ConvertWorldCoordsToMapCoords(position.x, position.y + GetScale().y);
		break;
	case Player::Positions::TopLeftIn:
		return ConvertWorldCoordsToMapCoords(position.x + upperInnerGap, position.y + GetScale().y - upperInnerGap);
		break;
	case Player::Positions::TopRightIn:
		return ConvertWorldCoordsToMapCoords(position.x + GetScale().x - upperInnerGap, position.y + GetScale().y - upperInnerGap);
		break;
	case Player::Positions::TopRightOut:
		return ConvertWorldCoordsToMapCoords(position.x + GetScale().x, position.y + GetScale().y);
		break;
	case Player::Positions::TopCenter:
		return ConvertWorldCoordsToMapCoords(position.x + (GetScale().x * 0.5f), position.y + GetScale().y - upperInnerGap);
		break;
	default:
		// If something went wrong, returns (-1, -1)
		return std::make_pair(-1, -1);
		break;
	}
}
