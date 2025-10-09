/*************************************************************************************************/
/*!
\file ActionManager.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.9.6
\brief
    Manages the player's actions.

    Functions include:
        + ActionManager
		+ ~ActionManager
		+ UpdateActions
		+ StartAction
		+ EndAction
		+ GetActionStatus

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_ActionManager_H_
#define Syncopatience_ActionManager_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base include
#include "../Engine/stdafx.h"

// The actions are stored in a vector list
#include <vector>

//-------------------------------------------------------------------------------------------------
// Forward References
//-------------------------------------------------------------------------------------------------

// Dependency reference
class GameObject;

//-------------------------------------------------------------------------------------------------
// Public Constants
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Classes
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Manages the players action
*/
/*************************************************************************************************/
class ActionManager
{
public:
	//---------------------------------------------------------------------------------------------
	// Public Consts
	//---------------------------------------------------------------------------------------------

	enum class ActionTypes
	{
		NullAction,
		Interact,
		Dash,
		DoubleJump,
		Max
	};

	enum class ActionPhase
	{
		NullAction = -1,
		Startup,
		Active,
		Ending,
		Max
	};
	
	//---------------------------------------------------------------------------------------------
	// Public Structures
	//---------------------------------------------------------------------------------------------

	typedef struct ActionStruct
	{
		ActionTypes actionType = ActionTypes::NullAction;	// The type of action
		ActionPhase actionPhase = ActionPhase::NullAction;	// The stage of this action
		int xCoord = 0;										// The X map coordinate of the action (Coordinate usage is action specific)
		int yCoord = 0;										// The y map coordinate of the action
		bool facingRight = true;							// Whether the action is aimed right or left
		double phaseTimer = 0.0;							// How much time is left in the current phase
		std::vector<GameObject*> targetList;				// After an action hits a target, tracks that game object so it isn't hit again
	}ActionStruct;
	
	//---------------------------------------------------------------------------------------------
	// Public Variables
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Function Declarations
	//---------------------------------------------------------------------------------------------

	/*************************************************************************************************/
	/*!
		\brief
			Constructor for the action manager class
	*/
	/*************************************************************************************************/
	ActionManager();

	/*************************************************************************************************/
	/*!
		\brief
			Destructor for action manager class
	*/
	/*************************************************************************************************/
	~ActionManager();

	/*************************************************************************************************/
	/*!
		\brief
			Updates all currently active actions checking within the given map matrix

		\param mapMatrix
			The given map matrix

		\param dt
			How much time elapsed since the previous frame
	*/
	/*************************************************************************************************/
	void UpdateActions(double dt);

	/*************************************************************************************************/
	/*!
		\brief
			Starts the given action regardless of current action status

		\param action
			The action to start

		\param actionXCoordinate
			The x coordinate of the action

		\param actionYCoordinate
			The y coordinate of the action

		\param actionFacingRight
			Boolean for whether the action is going to the right (true) or the left (false)
	*/
	/*************************************************************************************************/
	void StartAction(ActionTypes action, int actionXCoordinate, int actionYCoordinate, bool actionFacingRight);

	/*************************************************************************************************/
	/*!
		\brief
			Ends the given action regardless of current action status
	*/
	/*************************************************************************************************/
	void EndAction();

	/*************************************************************************************************/
	/*!
		\brief
			Returns the current action

		\return
			The current action
	*/
	/*************************************************************************************************/
	ActionStruct GetCurrentActionStatus();

	/*************************************************************************************************/
	/*!
		\brief
			Returns how long the given type of action takes to execute

		\param actionType
			The type of action

		\return
			The total time for the action in seconds
	*/
	/*************************************************************************************************/
	double GetActionLength(ActionTypes actionType);
	
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

	ActionStruct currentAction;					// The current action the player is performing
	
	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

	/*************************************************************************************************/
	/*!
		\brief
			Checks if an active action hits

		\param mapMatrix
			The map the action is in

		\param activeAction
			The action being checked

		\return
			Whether the action hit anything
	*/
	/*************************************************************************************************/
	bool CheckActiveAction(ActionStruct& activeAction);

	/*************************************************************************************************/
	/*!
		\brief
			Calculates tile coordinates with a left or right offset

		\param xCoord
			The X coordinate of the center tile

		\param yCoord
			The Y coordinate of the center tile

		\param facingRight
			Whether the action is facing right

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
			Returns the total action length of the given action

		\param action
			The action to check

		\return
			The action length
	*/
	/*************************************************************************************************/
	double CalculateTotalActionTime(ActionTypes action);
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_ActionManager_H_
