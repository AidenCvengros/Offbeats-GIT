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

// Base include
#include "../Engine/stdafx.h"

// The base game object class
#include "GameObject.h"

// The action manager class to keep track of what actions the player is taking
#include "../Gameplay/ActionManager.h"

// Includes the input manager because we need to check for specific inputs
#include "../Engine/InputManager.h"

//-------------------------------------------------------------------------------------------------
// Forward References
//-------------------------------------------------------------------------------------------------

// Dependency references
class MapMatrix;
class Inventory;

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

		\param mapCoords
			The starting coordinates of the player
	*/
	/*************************************************************************************************/
	Player(glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, Texture* texture_, std::pair<int, int> mapCoords);
	
	/*************************************************************************************************/
	/*!
		\brief
			Destructor for the player class
	*/
	/*************************************************************************************************/
	~Player();

	/*************************************************************************************************/
	/*!
		\brief
			Updates the game object.

		\param dt
			The time elapsed since the previous frame
	*/
	/*************************************************************************************************/
	void Update(double dt);

	/*************************************************************************************************/
	/*!
		\brief
			Gets the player's current velocity

		\return
			The player's current velocity (x, y)
	*/
	/*************************************************************************************************/
	std::pair<float, float> GetVelocity() { return std::make_pair(horizontalVelocity, verticalVelocity); }

	/*************************************************************************************************/
	/*!
		\brief
			Gets whether the player is grounded

		\return
			The player's grounded boolean
	*/
	/*************************************************************************************************/
	bool GetIsGrounded() { return grounded; }
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------

	enum class Positions
	{
		Center,
		BottomLeftOut,
		BottomLeftIn,
		BottomRightIn,
		BottomRightOut,
		TopLeftOut,
		TopLeftIn,
		TopRightIn,
		TopRightOut,
		TopCenter,
		Max
	};

	enum class PlayerStates
	{
		Walking,
		Running,
		Placing,
		Max
	};
	
	//---------------------------------------------------------------------------------------------
	// Private Structures
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Variables
	//---------------------------------------------------------------------------------------------
	
	double timeSinceMove;						// tracks how long since a movement started (to help buffer if you pressed a button late)
	float horizontalVelocity;					// How quickly the player is moving horizontally
	float verticalVelocity;						// How quickly the player is moving vertically
	bool grounded;								// Whether the player is grounded
	bool jumped;								// Boolean to keep the player from continually jumping if the button is held
	int againstWall;							// Whether the player is up against a wall
	bool goingMaxSpeed;							// Whether the player is going max speed
	float maxSpeed;								// The max speed of the player
	PlayerStates currentPlayerState;			// The current player state

	float lowerInnerGap;						// The distance between the outer and inner hitbox on the bottom of the player
	float upperInnerGap;						// The distance between the outer and inner hitbox on the top of the player

	ActionManager actionManager;				// Keeps track of actions the player uses

	Inventory* inventory;						// The player's inventory

	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

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
	bool CheckInput(InputManager::Inputs input);

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
	void AcceleratePlayerHorizontal(float accelerationAmount, double dt);

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
	void AcceleratePlayerVertical(float accelerationAmount, double dt);

	/*************************************************************************************************/
	/*!
		\brief
			Helper function to manage moving the player

		\param dt
			The time elapsed since the previous frame
	*/
	/*************************************************************************************************/
	void MovePlayer(double dt);

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
	std::pair<bool, bool> CollisionCheck(float horizontalMovement, float verticalMovement);

	/*************************************************************************************************/
	/*!
		\brief
			Helper function to check if the player has left the ground (usually by walking off a ledge)

		\return
			Whether the player left the ground
	*/
	/*************************************************************************************************/
	bool UngroundedCheck();

	/*************************************************************************************************/
	/*!
		\brief
			Helper function to update the coordinates of what square the player is in
	*/
	/*************************************************************************************************/
	void UpdatePlayerCoords();

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
	void Interact(double dt, std::pair<int, int>& playerPosition);

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
	void InteractWithTile(std::pair<int, int> targetTileCoords, bool destructibles, bool collectibles);
	void InteractWithTile(int targetTileXCoord, int targetTileYCoord, bool destructibles, bool collectibles) { InteractWithTile({ targetTileXCoord, targetTileYCoord }, destructibles, collectibles); }

	/*************************************************************************************************/
	/*!
		\brief
			Starts and manages the three hit basic action combo
	*/
	/*************************************************************************************************/
	//void ProgressBasicAction();

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
	std::pair<int, int> CalculatePlayerMapPositions(glm::vec2 position, Positions anchorPoint);
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_Player_H_
