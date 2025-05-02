/*************************************************************************************************/
/*!
\file Enemy.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.7.11
\brief
    The base game object class representing enemies

    Public Functions:
        + FILL
		
	Private Functions:
		+ FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_Enemy_H_
#define Syncopatience_Enemy_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "../stdafx.h"

#include "../GameObject.h"

#include "../MapMatrix.h"

#include "../Pool.h"

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
		The base enemy game object class
*/
/*************************************************************************************************/
class Enemy : public GameObject
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
	Enemy(glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, Texture* texture_, MapMatrix* mapMatrix_, std::pair<int, int> mapCoords);
	
	/*************************************************************************************************/
	/*!
		\brief
			Destructor for base enemy class
	*/
	/*************************************************************************************************/
	virtual ~Enemy();

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
	virtual void Update(double dt, InputManager* inputManager);

	/*************************************************************************************************/
	/*!
		\brief
			Draws the child game objects

		\param window
			The game window the objects are being drawn to
	*/
	/*************************************************************************************************/
	virtual void DrawChildObjects(Window* window);

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
	bool MoveEnemy(std::pair<int, int>& enemyPosition, int horizontalMove, int verticalMove, double moveSpeed);

	/*************************************************************************************************/
	/*!
		\brief
			Helper function to manage moving the enemy

		\param dt
			The time elapsed since the previous frame

		\param enemyPosition
			The current position of the enemy (will be modified if the enemy moves)
	*/
	/*************************************************************************************************/
	virtual void Attack(double dt, std::pair<int, int>& enemyPosition);

	/*************************************************************************************************/
	/*!
		\brief
			Deals the given amount of damage to the enemy

		\param damage
			The amount of damage to deal
	*/
	/*************************************************************************************************/
	void DamageEnemy(float damage);
	
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

	double attackTimer;							// The active time on the attack

	Pool* healthPool;							// The health pool of the enemy

	MapMatrix* mapMatrix;						// The map that the player is in

	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_Enemy_H_
