/*************************************************************************************************/
/*!
\file ActionManager.cpp
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
#include "ActionManager.h"
#include "../Engine/cppShortcuts.h"

// Includes the player and enemy classes to hit them
#include "../Game_Objects/Player.h"
#include "../Game_Objects/Enemy.h"

// Includes the Effects Manager so we can make action effects
#include "../Engine/EffectManager.h"

// Includes the map matrix class so actions can interact with the map
#include "MapMatrix.h"

//-------------------------------------------------------------------------------------------------
// Private Constants
//-------------------------------------------------------------------------------------------------

// The data structure to hold the qualities of an action
static struct ActionDataStruct
{
	double startupTime;
	double activeTime;
	double endingTime;

	float manaCost;
}actionDataStruct;

static const ActionDataStruct actionData[(unsigned long long)ActionManager::ActionTypes::Max] =
{
//  Startup  Active  Ending  Damage  Mana
	{0.0,	0.0,	0.0,	0.0f},			// Null Action
	{0.100, 0.066,  0.100,  0.0f},			// Interact
	{0.116, 0.066,  0.116,  0.0f},			// Dash
	{0.150, 0.100,  0.250,  0.0f},			// Double Jump
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
		Constructor for the action manager class
*/
/*************************************************************************************************/
ActionManager::ActionManager() : currentAction({ActionManager::ActionTypes::NullAction, ActionManager::ActionPhase::NullAction, -1, -1, true, 0.0f})
{
	
}

/*************************************************************************************************/
/*!
	\brief
		Destructor for action manager class
*/
/*************************************************************************************************/
ActionManager::~ActionManager()
{

}

/*************************************************************************************************/
/*!
	\brief
		Updates all currently active actions within the given map matrix

	\param dt
		How much time elapsed since the previous frame
*/
/*************************************************************************************************/
void ActionManager::UpdateActions(double dt)
{
	// Updates the current action
	if (currentAction.actionPhase != ActionPhase::NullAction)
	{
		// Decrements the phase timer
		currentAction.phaseTimer -= dt;

		// Checks if the action needs to progress phases
		if (currentAction.phaseTimer <= 0.0)
		{
			switch (currentAction.actionPhase)
			{
			case ActionPhase::Startup:
				currentAction.actionPhase = ActionPhase::Active;
				currentAction.phaseTimer = actionData[(int)currentAction.actionType].activeTime;
				break;
			case ActionPhase::Active:
				currentAction.actionPhase = ActionPhase::Ending;
				currentAction.phaseTimer = actionData[(int)currentAction.actionType].endingTime;
				break;
			case ActionPhase::Ending:
				currentAction.actionPhase = ActionPhase::NullAction;
				currentAction.phaseTimer = 0.0;
				EndAction();
				break;
			default:
				break;
			}
		}

		// If it's action phase, actions
		if (currentAction.actionPhase == ActionPhase::Active)
		{
			CheckActiveAction(currentAction);
		}
	}
}

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
void ActionManager::StartAction(ActionManager::ActionTypes action, int actionXCoordinate, int actionYCoordinate, bool actionFacingRight)
{
	//ActionStruct newAction = { action, ActionPhase::Startup, actionXCoordinate, actionYCoordinate, actionFacingRight, actionData[(int)action].startupTime };
	//currentAction = newAction;
	//
	//// Starts the slash1 effects
	//if (action == ActionTypes::Slash1)
	//{
	//	// Checks if the action is facing right or left
	//	glm::vec2 effectCoordinates = ConvertMapCoordsToWorldCoords( actionXCoordinate, actionYCoordinate);
	//	effectCoordinates.y += 0.2f;
	//	if (actionFacingRight)
	//	{
	//		effectCoordinates.x += 1.3f;
	//	}
	//	else
	//	{
	//		effectCoordinates.x -= 1.3f;
	//	}
	//
	//	_EffectManager->StartEffect(EffectManager::EffectType::Image, "Assets/Sprites/Slash1.png", effectCoordinates, 0.0f, { 1.5f, 1.5f }, actionFacingRight, CalculateTotalActionTime(action) + 0.25, { 0.9f, 0.3f, 0.1f, 0.85f }, 0.1);
	//}
	//// Starts the slash2 effects
	//if (action == ActionTypes::Slash2)
	//{
	//	// Checks if the action is facing right or left
	//	glm::vec2 effectCoordinates = ConvertMapCoordsToWorldCoords(actionXCoordinate, actionYCoordinate);
	//	effectCoordinates.y -= 0.2f;
	//	if (actionFacingRight)
	//	{
	//		effectCoordinates.x += 1.35f;
	//	}
	//	else
	//	{
	//		effectCoordinates.x -= 1.35f;
	//	}
	//
	//	_EffectManager->StartEffect(EffectManager::EffectType::Image, "Assets/Sprites/Slash1.png", effectCoordinates, 0.0f, { 1.5f, -1.5f }, actionFacingRight, CalculateTotalActionTime(action) + 0.25, { 0.9f, 0.3f, 0.1f, 0.85f }, 0.1);
	//}
	//// Starts the slash3 effects
	//if (action == ActionTypes::Slash3)
	//{
	//	// Checks if the action is facing right or left
	//	glm::vec2 effectCoordinates = ConvertMapCoordsToWorldCoords(actionXCoordinate, actionYCoordinate);
	//	if (actionFacingRight)
	//	{
	//		effectCoordinates.x += 1.4f;
	//	}
	//	else
	//	{
	//		effectCoordinates.x -= 1.4f;
	//	}
	//
	//	_EffectManager->StartEffect(EffectManager::EffectType::Image, "Assets/Sprites/Slash1.png", effectCoordinates, -10.0f, { 2.25f, 1.2f }, actionFacingRight, CalculateTotalActionTime(action) + 0.25, { 1.0f, 0.2f, 0.1f, 0.9f }, 0.2);
	//}
}

/*************************************************************************************************/
/*!
	\brief
		Ends the given action regardless of current action status
*/
/*************************************************************************************************/
void ActionManager::EndAction()
{
	currentAction = { ActionTypes::NullAction, ActionPhase::NullAction, -1, -1, false, 0.0f };
}

/*************************************************************************************************/
/*!
	\brief
		Returns the current action

	\return
		The current action
*/
/*************************************************************************************************/
ActionManager::ActionStruct ActionManager::GetCurrentActionStatus()
{
	return currentAction;
}

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
double ActionManager::GetActionLength(ActionTypes actionType)
{
	return actionData[(int)actionType].startupTime + actionData[(int)actionType].activeTime + actionData[(int)actionType].endingTime;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Checks if an active action hits

	\param activeAction
		The action being checked

	\return
		Whether the action hit anything
*/
/*************************************************************************************************/
bool ActionManager::CheckActiveAction(ActionManager::ActionStruct& activeAction)
{
	//// Checks hitboxes based on what type of action it is
	//if (activeAction.actionType == ActionTypes::Slash1)
	//{
	//	// Gets the tile status being hit
	//	// Slash1 hits just in front
	//	std::pair<int, int> hitTileCoords = CalculateOffsetTile(activeAction.xCoord, activeAction.yCoord, activeAction.facingRight, 1);
	//	MapMatrix::MapTile hitTile = _MapMatrix->GetTile(hitTileCoords.first, hitTileCoords.second);
	//
	//	// Checks if the action hit an enemy
	//	if (hitTile.tileStatus == MapMatrix::TileStatus::Enemy && dynamic_cast<Enemy*>(hitTile.tileObject) && std::find(activeAction.targetList.begin(), activeAction.targetList.end(), hitTile.tileObject) == activeAction.targetList.end())
	//	{
	//		((Enemy*)hitTile.tileObject)->DamageEnemy(4.0f);
	//		activeAction.targetList.push_back(hitTile.tileObject);
	//		return true;
	//	}
	//	// Checks if the action hit a destructible wall
	//	if (hitTile.tileStatus == MapMatrix::TileStatus::Destructible)
	//	{
	//		_MapMatrix->ClearTile(hitTileCoords.first, hitTileCoords.second);
	//		return true;
	//	}
	//}
	//
	//// Checks hitboxes based on what type of action it is
	//if (activeAction.actionType == ActionTypes::Slash2)
	//{
	//	// Gets the tile status being hit
	//	// Slash2 hits just in front
	//	std::pair<int, int> hitTileCoords = CalculateOffsetTile(activeAction.xCoord, activeAction.yCoord, activeAction.facingRight, 1);
	//	MapMatrix::MapTile hitTile = _MapMatrix->GetTile(hitTileCoords.first, hitTileCoords.second);
	//
	//	// Checks if the action hit an enemy
	//	if (hitTile.tileStatus == MapMatrix::TileStatus::Enemy && dynamic_cast<Enemy*>(hitTile.tileObject) && std::find(activeAction.targetList.begin(), activeAction.targetList.end(), hitTile.tileObject) == activeAction.targetList.end())
	//	{
	//		((Enemy*)hitTile.tileObject)->DamageEnemy(6.0f);
	//		activeAction.targetList.push_back(hitTile.tileObject);
	//		return true;
	//	}
	//	// Checks if the action hit a destructible wall
	//	if (hitTile.tileStatus == MapMatrix::TileStatus::Destructible)
	//	{
	//		_MapMatrix->ClearTile(hitTileCoords.first, hitTileCoords.second);
	//		return true;
	//	}
	//}
	//
	//// Checks hitboxes based on what type of action it is
	//if (activeAction.actionType == ActionTypes::Slash3)
	//{
	//	// Gets the tile status being hit
	//	// Slash1 hits just in front
	//	std::pair<int, int> hitTileCoords = CalculateOffsetTile(activeAction.xCoord, activeAction.yCoord, activeAction.facingRight, 1);
	//	MapMatrix::MapTile hitTile = _MapMatrix->GetTile(hitTileCoords.first, hitTileCoords.second);
	//
	//	// Checks if the action hit an enemy
	//	if (hitTile.tileStatus == MapMatrix::TileStatus::Enemy && dynamic_cast<Enemy*>(hitTile.tileObject) && std::find(activeAction.targetList.begin(), activeAction.targetList.end(), hitTile.tileObject) == activeAction.targetList.end())
	//	{
	//		((Enemy*)hitTile.tileObject)->DamageEnemy(10.0f);
	//		activeAction.targetList.push_back(hitTile.tileObject);
	//		return true;
	//	}
	//	// Checks if the action hit a destructible wall
	//	if (hitTile.tileStatus == MapMatrix::TileStatus::Destructible)
	//	{
	//		_MapMatrix->ClearTile(hitTileCoords.first, hitTileCoords.second);
	//		return true;
	//	}
	//}

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
		Whether the action is facing right

	\param xOffset
		The offset from the center tile. Will be used in conjunction with the facingRight variable to check the correct tile

	\return
		Returns the tile coordinates
*/
/*************************************************************************************************/
std::pair<int, int> ActionManager::CalculateOffsetTile(int xCoord, int yCoord, bool facingRight, int xOffset)
{
	// Checks if the action is facing right
	if (facingRight)
	{
		return std::make_pair(xCoord + xOffset, yCoord);
	}
	// Otherwise when the action is facing left
	else
	{
		return std::make_pair(xCoord - xOffset, yCoord);
	}
}

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
double ActionManager::CalculateTotalActionTime(ActionManager::ActionTypes action)
{
	return actionData[(int)action].startupTime + actionData[(int)action].activeTime + actionData[(int)action].endingTime;
}
