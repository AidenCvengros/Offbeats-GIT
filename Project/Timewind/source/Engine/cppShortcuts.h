/*************************************************************************************************/
/*!
\file cppShortcuts.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.12.5
\brief
    Include file for .cpp files that simplifies function calls, especially engine calls

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Timewind_cppShortcuts_H_
#define Timewind_cppShortcuts_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "Engine.h"

//-------------------------------------------------------------------------------------------------
// Forward References
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Constants
//-------------------------------------------------------------------------------------------------

#define _Window Engine::createEngine()->GetWindow()																			// Gets the window object
#define _CurrentScene Engine::createEngine()->GetCurrentScene()																// Gets the current scene
#define _MapMatrix ((MapMatrix*)Engine::createEngine()->GetSystem(Engine::SystemTypes::mapMatrix))							// Gets the active map matrix
#define _EffectManager ((EffectManager*)Engine::createEngine()->GetSystem(Engine::SystemTypes::effectManager))				// Gets the effect manager
#define _TextureManager ((TextureManager*)Engine::createEngine()->GetSystem(Engine::SystemTypes::textureManager))			// Gets the texture manager
#define _GameObjectManager ((GameObjectManager*)Engine::createEngine()->GetSystem(Engine::SystemTypes::gameObjectManager))	// Gets the game object manager
#define _InputManager ((InputManager*)Engine::createEngine()->GetSystem(Engine::SystemTypes::inputManager))					// Gets the input manager

//-------------------------------------------------------------------------------------------------
// Public Classes
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Converts a given map matrix tile coordinate to a world coordinate

	\param mapCoordinate
		The given map tile coordinate

	\return
		The corresponding location in world coordinates
*/
/*************************************************************************************************/
double ConvertMapCoordToWorldCoord(int mapCoordinate);

/*************************************************************************************************/
/*!
	\brief
		Converts the given map matrix tile coordinates to world coordinates

	\param mapXCoordinate
		The given map tile x coordinate

	\param mapYCoordinate
		The given map tile y coordinate

	\return
		The corresponding location in world coordinates
*/
/*************************************************************************************************/
glm::vec2 ConvertMapCoordsToWorldCoords(int mapXCoordinate, int mapYCoordinate);

/*************************************************************************************************/
/*!
	\brief
		Converts the given map matrix tile coordinates to world coordinates

	\param mapCoordinates
		The given map tile coordinates

	\return
		The corresponding location in world coordinates
*/
/*************************************************************************************************/
glm::vec2 ConvertMapCoordsToWorldCoords(std::pair<int, int> mapCoordinates);

/*************************************************************************************************/
/*!
	\brief
		Converts a given world matrix tile coordinate to a map coordinate

	\param worldCoordinate
		The given world tile coordinate

	\return
		The corresponding location in map coordinates
*/
/*************************************************************************************************/
int ConvertWorldCoordToMapCoord(double worldCoordinate);

/*************************************************************************************************/
/*!
	\brief
		Converts the given world matrix tile coordinates to map coordinates

	\param worldXCoordinate
		The given world tile x coordinate

	\param worldYCoordinate
		The given world tile y coordinate

	\return
		The corresponding location in map coordinates
*/
/*************************************************************************************************/
std::pair<int, int> ConvertWorldCoordsToMapCoords(double worldXCoordinate, double worldYCoordinate);

/*************************************************************************************************/
/*!
	\brief
		Converts the given world matrix tile coordinates to map coordinates

	\param worldCoordinates
		The given world coordinates

	\return
		The corresponding location in map coordinates
*/
/*************************************************************************************************/
std::pair<int, int> ConvertWorldCoordsToMapCoords(glm::vec2 worldCoordinates);

#endif // Timewind_cppShortcuts_H_
