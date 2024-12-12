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

// glm matrix and math functionality
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//-------------------------------------------------------------------------------------------------
// Forward References
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Constants
//-------------------------------------------------------------------------------------------------

#define _CurrentScene ((SceneManager*)Engine::createEngine()->GetSystem(Engine::SystemTypes::sceneManager))->GetCurrentScene()                      // Gets the current scene
#define _MapMatrix ((SceneManager*)Engine::createEngine()->GetSystem(Engine::SystemTypes::sceneManager))->GetMapMatrix()                            // Gets the active map matrix
#define _EffectManager ((EffectManager*)Engine::createEngine()->GetSystem(Engine::SystemTypes::effectManager))										// Gets the effect manager
#define _TextureManager ((TextureManager*)Engine::createEngine()->GetSystem(Engine::SystemTypes::textureManager))									// Gets the texture manager
#define _GameObjectManager ((GameObjectManager*)Engine::createEngine()->GetSystem(Engine::SystemTypes::gameObjectManager))							// Gets the game object manager

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

#endif // Timewind_cppShortcuts_H_
