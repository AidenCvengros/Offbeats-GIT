/*************************************************************************************************/
/*!
\file AttackManager.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.9.6
\brief
    Manages the player's attacks.

    Functions include:
        + AttackManager
		+ ~AttackManager
		+ UpdateAttacks
		+ StartAttack
		+ EndAttack
		+ GetAttackStatus

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_AttackManager_H_
#define Syncopatience_AttackManager_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"

#include <vector>

#include "MapMatrix.h"

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
		Manages the players attacks
*/
/*************************************************************************************************/
class AttackManager
{
public:
	//---------------------------------------------------------------------------------------------
	// Public Consts
	//---------------------------------------------------------------------------------------------

	enum class AttackTypes
	{
		NullAttack,
		Slash1,
		Slash2,
		Slash3,
		ConductingStrike,
		UpwardsSlash,
		Slamdown,
		Max
	};

	enum class AttackPhase
	{
		NullAttack = -1,
		Startup,
		Active,
		Ending,
		Max
	};
	
	//---------------------------------------------------------------------------------------------
	// Public Structures
	//---------------------------------------------------------------------------------------------

	typedef struct 
	{
		AttackTypes attackType;						// The type of attack
		AttackPhase attackPhase;				// The stage of this attack
		int xCoord;								// The X map coordinate of the attack (Coordinate usage is attack specific)
		int yCoord;								// The y map coordinate of the attack
		bool facingRight;						// Whether the attack is aimed right or left
		double phaseTimer;						// How much time is left in the current phase
		std::vector<GameObject*> targetList;	// After an attack hits a target, tracks that game object so it isn't hit again
	}AttackStruct;
	
	//---------------------------------------------------------------------------------------------
	// Public Variables
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Function Declarations
	//---------------------------------------------------------------------------------------------

	/*************************************************************************************************/
	/*!
		\brief
			Constructor for the attack manager class
	*/
	/*************************************************************************************************/
	AttackManager();

	/*************************************************************************************************/
	/*!
		\brief
			Destructor for attack manager class
	*/
	/*************************************************************************************************/
	~AttackManager();

	/*************************************************************************************************/
	/*!
		\brief
			Updates all currently active attacks checking within the given map matrix

		\param mapMatrix
			The given map matrix

		\param dt
			How much time elapsed since the previous frame
	*/
	/*************************************************************************************************/
	void UpdateAttacks(MapMatrix* mapMatrix, double dt);

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
	void StartAttack(AttackTypes attack, int attackXCoordinate, int attackYCoordinate, bool attackFacingRight);

	/*************************************************************************************************/
	/*!
		\brief
			Ends the given attack regardless of current attack status
	*/
	/*************************************************************************************************/
	void EndAttack();

	/*************************************************************************************************/
	/*!
		\brief
			Returns the current attack

		\return
			The current attack
	*/
	/*************************************************************************************************/
	AttackStruct GetCurrentAttackStatus();

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
	double GetAttackLength(AttackTypes attackType);
	
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

	AttackStruct currentAttack;					// The current attack the player is performing

	std::vector<AttackStruct> activeAttacks;	// Holds the active attacks (like projectiles that linger)
	
	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

	/*************************************************************************************************/
	/*!
		\brief
			Checks if an active attack hits

		\param mapMatrix
			The map the attack is in

		\param activeAttack
			The attack being checked

		\return
			Whether the attack hit anything
	*/
	/*************************************************************************************************/
	bool CheckActiveAttack(MapMatrix* mapMatrix, AttackStruct& activeAttack);

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
	std::pair<int, int> CalculateOffsetTile(int xCoord, int yCoord, bool facingRight, int xOffset);

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
	double CalculateTotalAttackTime(AttackManager::AttackTypes attack);
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_AttackManager_H_
