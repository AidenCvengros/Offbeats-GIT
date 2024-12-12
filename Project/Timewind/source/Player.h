/*************************************************************************************************/
/*!
\file Player.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.2.16
\brief
    The game object representing the player

    Public Functions:
        + FILL
		
	Private Functions:
		+ FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_Player_H_
#define Syncopatience_Player_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"

#include "GameObject.h"

#include "MapMatrix.h"

#include "AttackManager.h"

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
		The player game object
*/
/*************************************************************************************************/
class Player : public GameObject
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
			Constructor for the player game object class
			
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

		\param mapMatrix_
			The map that the player object is in

		\param mapCoords
			The starting coordinates of the player
	*/
	/*************************************************************************************************/
	Player(glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, Texture* texture_, MapMatrix* mapMatrix_, std::pair<int, int> mapCoords);
	
	/*************************************************************************************************/
	/*!
		\brief
			Destructor for FILL class
	*/
	/*************************************************************************************************/
	~Player() {}

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
	void Update(double dt, InputManager* inputManager);
	
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
	
	int jumpPhase;								// Tracks the progress of a jump
	std::pair<int, int> playerPrevPos;			// The previous player position in map coordinates
	double timeSinceMove;						// tracks how long since a movement started (to help buffer if you pressed a button late)

	int attackQueued;							// Denotes whether an attack has been pressed (0 is no attack, it should activate the next time the player is at a tile)
	bool jumpAttacked;							// Boolean to prevent the player from whiffing multiple times in a jump
	AttackManager attackManager;				// Keeps track of attacks the player uses

	MapMatrix* mapMatrix;						// The map that the player is in

	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

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
	bool CheckInput(InputManager* inputManager, InputManager::Inputs input);

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
	bool MovePlayer(std::pair<int, int>& playerPosition, int horizontalMove, int verticalMove, double moveSpeed);

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
	void Attack(double dt, std::pair<int, int>& playerPosition);

	/*************************************************************************************************/
	/*!
		\brief
			Starts and manages the three hit basic attack combo
	*/
	/*************************************************************************************************/
	void ProgressBasicAttack();
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_Player_H_
