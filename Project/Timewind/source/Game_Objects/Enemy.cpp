/*************************************************************************************************/
/*!
\file Enemy.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.7.11
\brief
    The base game object class representing enemies

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "Enemy.h"

#include "../cppShortcuts.h"

#include "../Window.h"

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
		Constructor for the base enemy game object class

	\param pos
		The position of the game object

	\param rot
		The rotation of the game object

	\param sca
		The scale of the game object

	\param drawPriority_
		Higher draw priorities are drawn in front of objects with lower priority

	\param texture_
		The texture of the enemy object

	\param mapMatrix_
		The map that the enemy object is in

	\param mapCoords
		The starting coordinates of the enemy
*/
/*************************************************************************************************/
Enemy::Enemy(glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, Texture* texture_, MapMatrix* mapMatrix_, std::pair<int, int> mapCoords) :
	GameObject(pos, rot, sca, drawPriority_, false, texture_, true, { 1.0f, 1.0f, 1.0f, 1.0f }, mapCoords),
	attackTimer(0.0),
	mapMatrix(mapMatrix_)
{
	healthPool = new Pool({ pos.x, pos.y + 0.5 }, rot, sca, 40, true, { 1.0, 0.0, 0.0, 0.7 });
	healthPool->SetRender(true);
}

/*************************************************************************************************/
/*!
	\brief
		Destructor for base enemy class
*/
/*************************************************************************************************/
Enemy::~Enemy()
{
	delete healthPool;
}

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
void Enemy::Update(double dt, InputManager* inputManager)
{
	// Updates movements if any are active
	if (GetMoving())
	{
		MoveToUpdate(dt);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Draws the child game objects

	\param window
		The game window the objects are being drawn to
*/
/*************************************************************************************************/
void Enemy::DrawChildObjects(Window* window)
{
	// Draws the enemy's health bar
	healthPool->SetPosition({ GetPosition().x, GetPosition().y + 1.3 });
	healthPool->SetScale({ 1.0f, 0.3f });
	healthPool->SetColor({ 1.0, 1.0, 1.0, 1.0 });
	window->DrawGameObject(healthPool);
	healthPool->SetPosition({ GetPosition().x, GetPosition().y + 1.3 });
	healthPool->SetScale({ healthPool->GetPoolRatio(), 0.3f });
	healthPool->SetColor({ 0.6, 0.1, 0.1, 1.0 });
	window->DrawGameObject(healthPool);
}

/*************************************************************************************************/
/*!
	\brief
		Helper function to manage moving the enemy

	\param enemyPosition
		The current position of the enemy (will be modified if the enemy moves)

	\param horizontalMove
		The horizontal movement (positive for right, negative for left)

	\param verticalMove
		The vertical movement (positive for up, negative for left)

	\param moveSpeed
		How long the movement takes

	\return
		Returns true if the move was successful, false if not (enemyPosition is not changed if false is returned)
*/
/*************************************************************************************************/
bool Enemy::MoveEnemy(std::pair<int, int>& enemyPosition, int horizontalMove, int verticalMove, double moveSpeed)
{
	// Corrects if the enemy is facing right or left
	if (horizontalMove > 0)
	{
		SetIsFacingRight(true);
	}
	else if (horizontalMove < 0)
	{
		SetIsFacingRight(false);
	}

	// Moves the enemy in logic
	if (mapMatrix->MoveTile(enemyPosition, { enemyPosition.first + horizontalMove, enemyPosition.second + verticalMove }, MapMatrix::TileStatus::Enemy, this))
	{
		enemyPosition.first += horizontalMove;
		enemyPosition.second += verticalMove;
		MoveTo(glm::vec2(ConvertMapCoordToWorldCoord(enemyPosition.first), ConvertMapCoordToWorldCoord(enemyPosition.second)), moveSpeed, false);

		// Since the move was successful, returns true
		return true;
	}

	// If the move failed, returns false
	return false;
}

/*************************************************************************************************/
/*!
	\brief
		Helper function to make the enemy attack

	\param dt
		The time elapsed since the previous frame

	\param enemyPosition
		The current position of the enemy (will be modified if the enemy moves)
*/
/*************************************************************************************************/
void Enemy::Attack(double dt, std::pair<int, int>& enemyPosition)
{
	// Checks if the enemy is open for an attack
	// Basic attack, hits ahead 1 spaces															UNFINISHED BEHAVIORS
	if (GetIsMoving() == false)
	{
		attackTimer = 0.2;
		MoveTo(GetPosition(), 0.2, false);
	}

	// Decrements the attack timer
	if (attackTimer > 0.0)
	{
		attackTimer -= dt;
	}
}

/*************************************************************************************************/
/*!
	\brief
		Deals the given amount of damage to the enemy

	\param damage
		The amount of damage to deal
*/
/*************************************************************************************************/
void Enemy::DamageEnemy(float damage)
{
	healthPool->SubtractFromPool(damage);

	// If the enemy health is at or below zero, destroy it
	if (healthPool->GetPoolValue() <= 0)
	{
		SetToBeDestroyed(true);
		mapMatrix->SetTile(GetMapCoords(), MapMatrix::TileStatus::Empty);
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
