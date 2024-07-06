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

#include "Player.h"

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
		Updates the game object.

	\param dt
		The time elapsed since the previous frame

	\param inputManager
		The input manager
*/
/*************************************************************************************************/
void Player::Update(double dt, InputManager* inputManager)
{
	// Updates movements if any are active
	if (GetMoving())
	{
		MoveToUpdate(dt);
	}

	std::pair playerPosition = mapMatrix->GetPlayerPosition();

	// The buffer zone
	if (GetIsMoving() == true)
	{
		// Jump buffer for having just started a grounded movement
		if (jumpPhase == 0 && timeSinceMove < 0.125 && inputManager->CheckInputStatus(InputManager::Inputs::Jump) == InputManager::InputStatus::Pressed)
		{
			if (MovePlayer(playerPrevPos, 0, 1, 0.07))
			{
				jumpPhase = 1;
			}
		}
	
		// Updates the time since move tracker
		timeSinceMove += dt;
	}

	// Checks for attacks being pressed
	if (inputManager->CheckInputStatus(InputManager::Inputs::Attack) == InputManager::InputStatus::Pressed)
	{
		attackQueued = 1;
	}

	// Performs attacks if queued
	Attack(dt, playerPosition);
	
	// Checks that the previous movement finished
	if (GetIsMoving() == false && jumpPhase == 0)
	{
		// Resets the grounded check for jump attacks
		jumpAttacked = false;
		attackQueued = 0;

		// Checks if the player isn't grounded
		if (mapMatrix->GetTile(playerPosition.first, playerPosition.second - 1) < MapMatrix::TileStatus::Player)
		{
			// Begin falling
			jumpPhase = 3;
		}

		// Checks for a jump (start of jump is quick up one tile
		else if (CheckInput(inputManager, InputManager::Inputs::Jump))
		{
			if (MovePlayer(playerPosition, 0, 1, 0.07))
			{
				jumpPhase = 1;
			}
		}

		// Checks for moving right
		else if (CheckInput(inputManager, InputManager::Inputs::Right))
		{
			MovePlayer(playerPosition, 1, 0, 0.25);
		}
		// Checks for moving left
		else if (CheckInput(inputManager, InputManager::Inputs::Left))
		{
			MovePlayer(playerPosition, -1, 0, 0.25);
		}
	}

	// Checks for end of jump phase 1 (float to apex of jump)
	if (GetIsMoving() == false && jumpPhase == 1)
	{
		// First checks if the space directly above is occupied (if it is we hit a ceiling and start falling)
		if (mapMatrix->GetTile(playerPosition.first, playerPosition.second + 1) == MapMatrix::TileStatus::Empty)
		{
			// Boolean to save on rewriting the code for jumping straight up if certain spaces are preventing sideways movement
			bool jumpUp = true;

			// Checks for moving right
			if (CheckInput(inputManager, InputManager::Inputs::Right))
			{
				// Checks if the space diagonally up is occupied (if it is we'll just treat it as a jump straight up)
				if (mapMatrix->GetTile(playerPosition.first + 1, playerPosition.second + 1) == MapMatrix::TileStatus::Empty)
				{
					// Attempt to move the player up and to the right
					if (MovePlayer(playerPosition, 1, 2, 0.225) == false)
					{
						// If the move was unsuccessful, then we move to the open space diagonally up
						MovePlayer(playerPosition, 1, 1, 0.15);
					}

					// Either way we should have moved, so we don't need the jump up routine
					jumpUp = false;
				}
			}
			// Checks for moving left
			else if (CheckInput(inputManager, InputManager::Inputs::Left))
			{
				// Checks if the space diagonally up is occupied (if it is we'll just treat it as a jump straight up)
				if (mapMatrix->GetTile(playerPosition.first - 1, playerPosition.second + 1) == MapMatrix::TileStatus::Empty)
				{
					// Attempt to move the player up and to the left
					if (MovePlayer(playerPosition, -1, 2, 0.225) == false)
					{
						// If the move was unsuccessful, then we move to the open space diagonally up
						MovePlayer(playerPosition, -1, 1, 0.15);
					}

					// Either way we should have moved, so we don't need the jump up routine
					jumpUp = false;
				}
			}
			// Checks for moving up
			if (jumpUp)
			{
				// Attempts to move the full distance up
				if (MovePlayer(playerPosition, 0, 2, 0.225) == false)
				{
					// If the full upward movement fails, only goes up one space
					MovePlayer(playerPosition, 0, 1, 0.15);
				}
			}
		}

		jumpPhase = 2;
	}
	// Checks for end of jump phase 2 (apex of jump)
	if (GetIsMoving() == false && jumpPhase == 2)
	{
		// Checks for moving right
		if (CheckInput(inputManager, InputManager::Inputs::Right))
		{
			MovePlayer(playerPosition, 1, 0, 0.15);
		}
		// Checks for moving left
		else if (CheckInput(inputManager, InputManager::Inputs::Left))
		{
			MovePlayer(playerPosition, -1, 0, 0.15);
		}
		// Otherwise hangs in the air
		else
		{
			MoveTo(GetPosition(), 0.15, false);
		}

		// Afterwards continues the jump
		jumpPhase = 3;
	}
	// Checks for end of jump phase 3 (begin falling)
	if (GetIsMoving() == false && jumpPhase == 3)
	{
		// First checks if the space directly below is occupied (if it is we're grounded)
		if (mapMatrix->GetTile(playerPosition.first, playerPosition.second - 1) != MapMatrix::TileStatus::Empty)
		{
			jumpPhase = 0;
		}
		// Otherwise falls
		else
		{
			// Boolean to save on rewriting the code for falling straight down if certain spaces are preventing sideways movement
			bool fallDown = true;

			// Checks for moving right
			if (CheckInput(inputManager, InputManager::Inputs::Right))
			{
				// Checks if the space diagonally down is occupied (if it is we'll just treat it as a fall straight down)
				if (mapMatrix->GetTile(playerPosition.first + 1, playerPosition.second - 1) == MapMatrix::TileStatus::Empty)
				{
					// Attempt to move the player down and to the right
					if (MovePlayer(playerPosition, 1, -2, 0.225) == false)
					{
						// If the move was unsuccessful, then we move to the open space diagonally down
						MovePlayer(playerPosition, 1, -1, 0.15);
					}

					// Either way we should have moved, so we don't need the fall down routine
					fallDown = false;
				}
			}
			// Checks for moving left
			else if (CheckInput(inputManager, InputManager::Inputs::Left))
			{
				// Checks if the space diagonally down is occupied (if it is we'll just treat it as a fall straight down)
				if (mapMatrix->GetTile(playerPosition.first - 1, playerPosition.second + 1) == MapMatrix::TileStatus::Empty)
				{
					// Attempt to move the player down and to the left
					if (MovePlayer(playerPosition, -1, -2, 0.225) == false)
					{
						// If the move was unsuccessful, then we move to the open space diagonally down
						MovePlayer(playerPosition, -1, -1, 0.15);
					}

					// Either way we should have moved, so we don't need the fall down routine
					fallDown = false;
				}
			}
			// Checks for falling straight down
			if (fallDown)
			{
				// Attempts to move the full distance down
				if (MovePlayer(playerPosition, 0, -2, 0.225) == false)
				{
					// If the full downward movement fails, only goes down one space
					MovePlayer(playerPosition, 0, -1, 0.15);
				}
			}

			// Marks that jump phase 3 has been resolved
			jumpPhase = 4;
		}
	}
	// Checks for end of jump phase 4 (Quick fall into possible landing)
	if (GetIsMoving() == false && jumpPhase == 4)
	{
		// Attempts to move down
		if (MovePlayer(playerPosition, 0, -1, 0.07) == false)
		{
			// If there is ground below, becomes grounded
			jumpPhase = 0;
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

	\param inputManager
		The input manager

	\param input
		The given input

	\return
		Whether the input was pressed or held
*/
/*************************************************************************************************/
bool Player::CheckInput(InputManager* inputManager, InputManager::Inputs input)
{
	return inputManager->CheckInputStatus(input) == InputManager::InputStatus::Pressed
		|| inputManager->CheckInputStatus(input) == InputManager::InputStatus::Held;
}

/*************************************************************************************************/
/*!
	\brief
		Helper function to manage moving the player

	\param playerPosition
		The current position of the player (will be modified if the player moves)

	\param horizontalMove
		The horizontal movement (positive for right, negative for left)

	\param verticalMove
		The vertical movement (positive for up, negative for left)

	\param moveSpeed
		How long the movement takes

	\return
		Returns true if the move was successful, false if not (playerPosition is not changed if false is returned)
*/
/*************************************************************************************************/
bool Player::MovePlayer(std::pair<int, int>& playerPosition, int horizontalMove, int verticalMove, double moveSpeed)
{
	// Corrects if the player is facing right or left
	if (horizontalMove > 0)
	{
		facingRight = true;
	}
	else if (horizontalMove < 0)
	{
		facingRight = false;
	}

	// Moves the player in logic
	if (mapMatrix->SetPlayerPosition(playerPosition.first + horizontalMove, playerPosition.second + verticalMove))
	{
		playerPrevPos = playerPosition;
		playerPosition.first += horizontalMove;
		playerPosition.second += verticalMove;
		MoveTo(glm::vec2(mapMatrix->GetMinMapX() + (2.0f * playerPosition.first), mapMatrix->GetMinMapY() - (2.0f * playerPosition.second)), moveSpeed, false);

		// Resets time since moving
		timeSinceMove = 0.0;

		// Since the move was successful, returns true
		return true;
	}

	// If the move failed, returns false
	return false;
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
void Player::Attack(double dt, std::pair<int, int>& playerPosition)
{
	// Checks if the player is open for an attack
	// Basic attack, hits ahead 2 spaces															UNFINISHED BEHAVIORS
	if (attackQueued != 0 && GetIsMoving() == false && jumpAttacked == false)
	{
		attackTimer = 0.2;
		attackQueued = 0;
		MoveTo(GetPosition(), 0.2, false);

		// Checks if the player is jumping
		if (jumpPhase > 0)
		{
			jumpAttacked = true;
		}
	}

	// Decrements the attack timer
	if (attackTimer > 0.0)
	{
		attackTimer -= dt;

		// Checks if the player is attacking to the right or left
		if (facingRight)
		{
			if (mapMatrix->GetTile(playerPosition.first + 1, playerPosition.second) == MapMatrix::TileStatus::Enemy)
			{

			}
			if (mapMatrix->GetTile(playerPosition.first + 1, playerPosition.second) == MapMatrix::TileStatus::Destructible)
			{
				mapMatrix->SetTile(playerPosition.first + 1, playerPosition.second, MapMatrix::TileStatus::Empty);
			}
		}
		// Otherwise does attacks facing left
		else
		{
			if (mapMatrix->GetTile(playerPosition.first - 1, playerPosition.second) == MapMatrix::TileStatus::Enemy)
			{

			}
			if (mapMatrix->GetTile(playerPosition.first - 1, playerPosition.second) == MapMatrix::TileStatus::Destructible)
			{
				mapMatrix->SetTile(playerPosition.first - 1, playerPosition.second, MapMatrix::TileStatus::Empty);
				jumpAttacked = 0;
			}
		}
	}
}
