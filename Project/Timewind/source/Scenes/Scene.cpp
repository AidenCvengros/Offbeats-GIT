/*************************************************************************************************/
/*!
\file Scene.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.5.22
\brief
    The base scene class to facilitate scene functionality

    Functions include:
        + [FILL]

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Scene.h"
#include "../Engine/cppShortcuts.h"

// Includes game object class to draw default square
#include "../Game_Objects/GameObject.h"

// Additional includes
#include <vector>
#include "../Gameplay/MapMatrix.h"
#include "../Engine/SceneManager.h"
//#include "../Engine/TextureManager.h"
//#include "../Engine/GameObjectManager.h"
//#include "../Game_Objects/Player.h"
//#include "../Game_Objects/FinishFlag.h"
//#include "../Game_Objects/DestructibleWall.h"
//#include "../Game_Objects/Coin.h"
//#include "../Game_Objects/BigCoin.h"

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
		The Scene constructor

	\param _sceneID
		The id for this scene
*/
/*************************************************************************************************/
Scene::Scene(int _sceneID) : inUse(false), sceneID(_sceneID), defaultSquare(NULL)
{
	
}

/*************************************************************************************************/
/*!
	\brief
		Deconstructor for the base scene class
*/
/*************************************************************************************************/
Scene::~Scene()
{

}

/*************************************************************************************************/
/*!
	\brief
		Refreshes the scene
*/
/*************************************************************************************************/
void Scene::RefreshScene()
{
    _SceneManager->ChangeScene(sceneID);
}

/*************************************************************************************************/
/*!
	\brief
		Draws a square of the given color at the given tile

	\param tileCoords
		The coordinates of the tile being drawn.

	\param color
		The color of square to draw
*/
/*************************************************************************************************/
void Scene::DrawTile(std::pair<int, int> coords, glm::vec4 color)
{
	// Checks that there is a default square to draw
	if (defaultSquare)
	{
		defaultSquare->SetPosition(ConvertMapCoordsToWorldCoords(coords));
		defaultSquare->SetColor(color);
		defaultSquare->DrawThisFrame(true);
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
