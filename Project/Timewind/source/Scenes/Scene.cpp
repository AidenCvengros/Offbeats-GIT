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
    //// Fetches textures
    //Texture* playerTexture = _TextureManager->AddTexture("Assets/Sprites/Alice_Neutral.png");
    //Texture* wallTexture = _TextureManager->AddTexture("Assets/Sprites/Wall2.png");
    //Texture* destructibleWallTexture = _TextureManager->AddTexture("Assets/Sprites/Wall_Cracked.png");
    //Texture* keyTexture = _TextureManager->AddTexture("Assets/Sprites/Key.png");
    //Texture* lockedWallTexture = _TextureManager->AddTexture("Assets/Sprites/LockedWall.png");
    //Texture* coinTexture = _TextureManager->AddTexture("Assets/Sprites/Coin.png");
    //Texture* bumperTexture = _TextureManager->AddTexture("Assets/Sprites/Bumper.png");
    //Texture* flagTexture = _TextureManager->AddTexture("Assets/Sprites/Flag.png");
    //
    //// Sets the map for the scene
    //std::vector< std::pair< char, std::pair< int, int > > > specialTileList;
    //_MapMatrix->ReloadMap(specialTileList, false);
    //
    //// Resets the player and camera positions
    //glm::vec2 playerPosition = ConvertMapCoordsToWorldCoords(_MapMatrix->GetPlayerPosition());
    //_GameObjectManager->GetPlayer()->SetPosition(playerPosition);
    //_GameObjectManager->GetPlayer()->ResetVelocity();
    //
    //// Creates a new finish flag
    //FinishFlag* finishFlag = new FinishFlag(flagTexture, { 1.0f, 1.0f, 1.0f, 1.0f }, { -1, -1 }, 0);
    //finishFlag->SetScale({ 4.0f, 12.0f });
    //finishFlag->SetTimer(20.0f);
    //_GameObjectManager->AddGameObject(finishFlag);
    //
    //// Adds in all special tiles
    //for (auto i = specialTileList.begin(); i != specialTileList.end(); i++)
    //{
    //    if (_MapMatrix->GetTile(i->second).tileObject == NULL)
    //    {
    //        GameObject* newObject = NULL;           // Game object pointer to store newly made objects
    //
    //        // Checks what character marked the special tile
    //        switch (i->first)
    //        {
    //        case 'd':
    //            newObject = new DestructibleWall(NULL, 0, destructibleWallTexture, { 0.4f, 0.075f, 0.0f, 1.0f }, i->second);
    //            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Destructible, newObject);
    //            break;
    //            // Generic coin
    //        case 'c':
    //            newObject = new Coin(coinTexture, { 1.0f, 1.0f, 1.0f, 1.0f }, i->second);
    //            newObject->SetScale(glm::vec2(1.25, 1.25));
    //            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Coin, newObject);
    //            break;
    //        case 'F':
    //            finishFlag->SetPosition(ConvertMapCoordsToWorldCoords(i->second, finishFlag->GetScale()));
    //            finishFlag->SetMapCoords(i->second);
    //            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::FinishFlag, finishFlag);
    //            break;
    //        case 'f':
    //            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::FinishFlag, finishFlag);
    //            break;
    //            // This was a bumper hidden in a block
    //        case 'B':
    //            newObject = new DestructibleWall(NULL, 0, destructibleWallTexture, { 0.4f, 0.075f, 0.0f, 1.0f }, i->second);
    //            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Destructible, newObject);
    //            break;
    //            // This is a coin hidden in a block
    //        case 'D':
    //            newObject = new Coin(coinTexture, { 1.0f, 1.0f, 1.0f, 1.0f }, i->second);
    //            newObject = new DestructibleWall((Item*)newObject, 0, destructibleWallTexture, { 0.4f, 0.075f, 0.0f, 1.0f }, i->second);
    //            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Destructible, newObject);
    //            break;
    //        default:
    //            break;
    //        }
    //
    //        // Adds the new object to the object manager
    //        if (newObject)
    //        {
    //            _GameObjectManager->AddGameObject(newObject);
    //        }
    //    }
    //    // If there is already an object there
    //    else
    //    {
    //        // Checks for finish zone
    //        if (i->first == 'f')
    //        {
    //            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::FinishFlag, finishFlag);
    //        }
    //        // Checks for finish flag
    //        if (i->first == 'F')
    //        {
    //            // Deletes the old flag
    //            _MapMatrix->GetTile(i->second).tileObject->SetToBeDestroyed(true);
    //
    //            // Sets the new flag
    //            finishFlag->SetPosition(ConvertMapCoordsToWorldCoords(i->second, finishFlag->GetScale()));
    //            finishFlag->SetMapCoords(i->second);
    //            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::FinishFlag, finishFlag);
    //        }
    //    }
    //}
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
