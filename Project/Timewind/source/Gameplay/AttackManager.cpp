/*************************************************************************************************/
/*!
\file AttackManager.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.9.6
\brief
    [FILL]

    Functions include:
        + [FILL]

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "AttackManager.h"
#include "../Engine/cppShortcuts.h"

// Includes the player and enemy classes to hit them
#include "../Game_Objects/Player.h"
#include "../Game_Objects/Enemy.h"

// Includes the Effects Manager so we can make attack effects
#include "../Engine/EffectManager.h"

// Includes the map matrix class so attacks can interact with the map
#include "MapMatrix.h"

//-------------------------------------------------------------------------------------------------
// Private Constants
//-------------------------------------------------------------------------------------------------

// The data structure to hold the qualities of an attack
static struct AttackDataStruct
{
	double startupTime;
	double activeTime;
	double endingTime;

	float damage;
	float manaCost;
}attackDataStruct;

static const AttackDataStruct attackData[(unsigned long long)AttackManager::AttackTypes::Max] =
{
//  Startup  Active  Ending  Damage  Mana
	{0.100,  0.066,  0.100,  4.0f,   0.0f},			// Slash 1
	{0.116,  0.066,  0.116,  6.0f,   0.0f},			// Slash 2
	{0.150,  0.100,  0.250,  10.0f,  0.0f},			// Slash 3
	{0.066,  0.150,  0.100,  12.0f,  10.0f},		// Conducting Strike
	{0.066,  0.100,  0.200,  16.0f,  4.0f},			// Upwards Slash	
	{0.150,  1.000,  0.300,  20.0f,  10.0f}			// Electric Slamdown
};

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
		Constructor for the attack manager class
*/
/*************************************************************************************************/
AttackManager::AttackManager() : currentAttack({AttackManager::AttackTypes::NullAttack, AttackManager::AttackPhase::NullAttack, -1, -1, true, 0.0f}), activeAttacks(8)
{
	
}

/*************************************************************************************************/
/*!
	\brief
		Destructor for attack manager class
*/
/*************************************************************************************************/
AttackManager::~AttackManager()
{

}

/*************************************************************************************************/
/*!
	\brief
		Updates all currently active attacks within the given map matrix

	\param dt
		How much time elapsed since the previous frame
*/
/*************************************************************************************************/
void AttackManager::UpdateAttacks(double dt)
{
	// Updates the current attack
	if (currentAttack.attackPhase != AttackPhase::NullAttack)
	{
		// Decrements the phase timer
		currentAttack.phaseTimer -= dt;

		// Checks if the attack needs to progress phases
		if (currentAttack.phaseTimer <= 0.0)
		{
			switch (currentAttack.attackPhase)
			{
			case AttackPhase::Startup:
				currentAttack.attackPhase = AttackPhase::Active;
				currentAttack.phaseTimer = attackData[(int)currentAttack.attackType].activeTime;
				break;
			case AttackPhase::Active:
				currentAttack.attackPhase = AttackPhase::Ending;
				currentAttack.phaseTimer = attackData[(int)currentAttack.attackType].endingTime;
				break;
			case AttackPhase::Ending:
				currentAttack.attackPhase = AttackPhase::NullAttack;
				currentAttack.phaseTimer = 0.0;
				EndAttack();
				break;
			default:
				break;
			}
		}

		// If it's attack phase, attacks
		if (currentAttack.attackPhase == AttackPhase::Active)
		{
			CheckActiveAttack(currentAttack);
		}
	}
}

/*************************************************************************************************/
/*!
	\brief
		Starts the given attack regardless of current attack status

	\param attack
		The attack to start

	\param attackXCoordinate
		The x coordinate of the attack

	\param attackYCoordinate
		The y coordinate of the attack

	\param attackFacingRight
		Boolean for whether the attack is going to the right (true) or the left (false)
*/
/*************************************************************************************************/
void AttackManager::StartAttack(AttackManager::AttackTypes attack, int attackXCoordinate, int attackYCoordinate, bool attackFacingRight)
{
	AttackStruct newAttack = { attack, AttackPhase::Startup, attackXCoordinate, attackYCoordinate, attackFacingRight, attackData[(int)attack].startupTime };
	currentAttack = newAttack;

	// Starts the slash1 effects
	if (attack == AttackTypes::Slash1)
	{
		// Checks if the attack is facing right or left
		glm::vec2 effectCoordinates = ConvertMapCoordsToWorldCoords( attackXCoordinate, attackYCoordinate);
		effectCoordinates.y += 0.2f;
		if (attackFacingRight)
		{
			effectCoordinates.x += 1.3f;
		}
		else
		{
			effectCoordinates.x -= 1.3f;
		}

		_EffectManager->StartEffect(EffectManager::EffectType::Image, "Assets/Sprites/Slash1.png", effectCoordinates, 0.0f, { 1.5f, 1.5f }, attackFacingRight, CalculateTotalAttackTime(attack) + 0.25, { 0.9f, 0.3f, 0.1f, 0.85f }, 0.1);
	}
	// Starts the slash2 effects
	if (attack == AttackTypes::Slash2)
	{
		// Checks if the attack is facing right or left
		glm::vec2 effectCoordinates = ConvertMapCoordsToWorldCoords(attackXCoordinate, attackYCoordinate);
		effectCoordinates.y -= 0.2f;
		if (attackFacingRight)
		{
			effectCoordinates.x += 1.35f;
		}
		else
		{
			effectCoordinates.x -= 1.35f;
		}

		_EffectManager->StartEffect(EffectManager::EffectType::Image, "Assets/Sprites/Slash1.png", effectCoordinates, 0.0f, { 1.5f, -1.5f }, attackFacingRight, CalculateTotalAttackTime(attack) + 0.25, { 0.9f, 0.3f, 0.1f, 0.85f }, 0.1);
	}
	// Starts the slash3 effects
	if (attack == AttackTypes::Slash3)
	{
		// Checks if the attack is facing right or left
		glm::vec2 effectCoordinates = ConvertMapCoordsToWorldCoords(attackXCoordinate, attackYCoordinate);
		if (attackFacingRight)
		{
			effectCoordinates.x += 1.4f;
		}
		else
		{
			effectCoordinates.x -= 1.4f;
		}

		_EffectManager->StartEffect(EffectManager::EffectType::Image, "Assets/Sprites/Slash1.png", effectCoordinates, -10.0f, { 2.25f, 1.2f }, attackFacingRight, CalculateTotalAttackTime(attack) + 0.25, { 1.0f, 0.2f, 0.1f, 0.9f }, 0.2);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Ends the given attack regardless of current attack status
*/
/*************************************************************************************************/
void AttackManager::EndAttack()
{
	currentAttack = { AttackTypes::NullAttack, AttackPhase::NullAttack, -1, -1, false, 0.0f };
}

/*************************************************************************************************/
/*!
	\brief
		Returns the current attack

	\return
		The current attack
*/
/*************************************************************************************************/
AttackManager::AttackStruct AttackManager::GetCurrentAttackStatus()
{
	return currentAttack;
}

/*************************************************************************************************/
/*!
	\brief
		Returns how long the given type of attack takes to execute

	\param attackType
		The type of attack

	\return
		The total time for the attack in seconds
*/
/*************************************************************************************************/
double AttackManager::GetAttackLength(AttackTypes attackType)
{
	return attackData[(int)attackType].startupTime + attackData[(int)attackType].activeTime + attackData[(int)attackType].endingTime;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Checks if an active attack hits

	\param activeAttack
		The attack being checked

	\return
		Whether the attack hit anything
*/
/*************************************************************************************************/
bool AttackManager::CheckActiveAttack(AttackManager::AttackStruct& activeAttack)
{
	// Checks hitboxes based on what type of attack it is
	if (activeAttack.attackType == AttackTypes::Slash1)
	{
		// Gets the tile status being hit
		// Slash1 hits just in front
		std::pair<int, int> hitTileCoords = CalculateOffsetTile(activeAttack.xCoord, activeAttack.yCoord, activeAttack.facingRight, 1);
		MapMatrix::MapTile hitTile = _MapMatrix->GetTile(hitTileCoords.first, hitTileCoords.second);

		// Checks if the attack hit an enemy
		if (hitTile.tileStatus == MapMatrix::TileStatus::Enemy && dynamic_cast<Enemy*>(hitTile.tileObject) && std::find(activeAttack.targetList.begin(), activeAttack.targetList.end(), hitTile.tileObject) == activeAttack.targetList.end())
		{
			((Enemy*)hitTile.tileObject)->DamageEnemy(4.0f);
			activeAttack.targetList.push_back(hitTile.tileObject);
			return true;
		}
		// Checks if the attack hit a destructible wall
		if (hitTile.tileStatus == MapMatrix::TileStatus::Destructible)
		{
			_MapMatrix->ClearTile(hitTileCoords.first, hitTileCoords.second);
			return true;
		}
	}

	// Checks hitboxes based on what type of attack it is
	if (activeAttack.attackType == AttackTypes::Slash2)
	{
		// Gets the tile status being hit
		// Slash2 hits just in front
		std::pair<int, int> hitTileCoords = CalculateOffsetTile(activeAttack.xCoord, activeAttack.yCoord, activeAttack.facingRight, 1);
		MapMatrix::MapTile hitTile = _MapMatrix->GetTile(hitTileCoords.first, hitTileCoords.second);

		// Checks if the attack hit an enemy
		if (hitTile.tileStatus == MapMatrix::TileStatus::Enemy && dynamic_cast<Enemy*>(hitTile.tileObject) && std::find(activeAttack.targetList.begin(), activeAttack.targetList.end(), hitTile.tileObject) == activeAttack.targetList.end())
		{
			((Enemy*)hitTile.tileObject)->DamageEnemy(6.0f);
			activeAttack.targetList.push_back(hitTile.tileObject);
			return true;
		}
		// Checks if the attack hit a destructible wall
		if (hitTile.tileStatus == MapMatrix::TileStatus::Destructible)
		{
			_MapMatrix->ClearTile(hitTileCoords.first, hitTileCoords.second);
			return true;
		}
	}

	// Checks hitboxes based on what type of attack it is
	if (activeAttack.attackType == AttackTypes::Slash3)
	{
		// Gets the tile status being hit
		// Slash1 hits just in front
		std::pair<int, int> hitTileCoords = CalculateOffsetTile(activeAttack.xCoord, activeAttack.yCoord, activeAttack.facingRight, 1);
		MapMatrix::MapTile hitTile = _MapMatrix->GetTile(hitTileCoords.first, hitTileCoords.second);

		// Checks if the attack hit an enemy
		if (hitTile.tileStatus == MapMatrix::TileStatus::Enemy && dynamic_cast<Enemy*>(hitTile.tileObject) && std::find(activeAttack.targetList.begin(), activeAttack.targetList.end(), hitTile.tileObject) == activeAttack.targetList.end())
		{
			((Enemy*)hitTile.tileObject)->DamageEnemy(10.0f);
			activeAttack.targetList.push_back(hitTile.tileObject);
			return true;
		}
		// Checks if the attack hit a destructible wall
		if (hitTile.tileStatus == MapMatrix::TileStatus::Destructible)
		{
			_MapMatrix->ClearTile(hitTileCoords.first, hitTileCoords.second);
			return true;
		}
	}

	// If nothing was hit, returns false
	return false;
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

	\return
		Returns the tile coordinates
*/
/*************************************************************************************************/
std::pair<int, int> AttackManager::CalculateOffsetTile(int xCoord, int yCoord, bool facingRight, int xOffset)
{
	// Checks if the attack is facing right
	if (facingRight)
	{
		return std::make_pair(xCoord + xOffset, yCoord);
	}
	// Otherwise when the attack is facing left
	else
	{
		return std::make_pair(xCoord - xOffset, yCoord);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Returns the total attack length of the given attack

	\param attack
		The attack to check

	\return
		The attack length
*/
/*************************************************************************************************/
double AttackManager::CalculateTotalAttackTime(AttackManager::AttackTypes attack)
{
	return attackData[(int)attack].startupTime + attackData[(int)attack].activeTime + attackData[(int)attack].endingTime;
}
