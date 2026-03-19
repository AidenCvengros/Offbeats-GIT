/*************************************************************************************************/
/*!
\file Level1.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.3.14
\brief
    The first level scene

    Functions include:
        + [FILL]

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Level1.h"
#include "../Engine/cppShortcuts.h"

// Includes game object manager and texture manager for creating new game objects in the scene
#include "../Engine/GameObjectManager.h"
#include "../Engine/TextureManager.h"
#include "../Game_Objects/Enemy.h"
#include "../Game_Objects/DestructibleWall.h"
#include "../Game_Objects/Key.h"
#include "../Game_Objects/LockedWall.h"
#include "../Game_Objects/Coin.h"
#include "../Game_Objects/ScreenCrack.h"
#include "../Game_Objects/Player.h"
#include "../Game_Objects/Camera.h"
#include "../Game_Objects/Stickers/Bumper.h"
#include "../Game_Objects/Stickers/BlockSticker.h"
#include "../Game_Objects/Stickers/Teleporter.h"
#include "../Game_Objects/BigCoin.h"

// Includes the map matrix class
#include "../Gameplay/MapMatrix.h"

// Includes the window class to initialize the camera object
#include "../Engine/Window.h"

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
        Loads in all the objects of the scene
*/
/*************************************************************************************************/
void Level1::LoadScene()
{
    // Creates textures
    Texture* playerTexture = _TextureManager->AddTexture("Assets/Sprites/Alice_Neutral.png");
    Texture* wallTexture = _TextureManager->AddTexture("Assets/Sprites/Wall2.png");
    Texture* destructibleWallTexture = _TextureManager->AddTexture("Assets/Sprites/Wall_Cracked.png");
    Texture* keyTexture = _TextureManager->AddTexture("Assets/Sprites/Key.png");
    Texture* lockedWallTexture = _TextureManager->AddTexture("Assets/Sprites/LockedWall.png");
    Texture* coinTexture = _TextureManager->AddTexture("Assets/Sprites/Coin.png");
    Texture* bumperTexture = _TextureManager->AddTexture("Assets/Sprites/Bumper.png");

    // Sets the default wall for the scene
    _MapMatrix->SetDefaultWallTexture(wallTexture, { 0.4f, 0.075f, 0.0f, 1.0f });

    // Sets the map for the scene
    std::vector< std::pair< char, std::pair< int, int > > > specialTileList;
    _MapMatrix->ReadMapFromFile("Assets/Maps/Level1.csv", specialTileList);

    // Creates essential game objects (player, camera, and default square)
    Player* player = new Player(ConvertMapCoordsToWorldCoords(_MapMatrix->GetPlayerPosition()), 0.0f, {2.0f, 2.0f}, 50, playerTexture, {2, 2});
    Camera* camera = new Camera(player->GetPosition(), 0.0f, glm::vec2(0.0f, 0.0f), player, _Window->GetWindowSize().x / _Window->GetWindowSize().y, glm::radians(50.0f));
    GameObject* newDefaultSquare = new GameObject({ 0.0f, 0.0f }, 0.0f, { 2.0f, 2.0f }, 99, true, { 1.0f, 1.0f, 1.0f, 1.0f }, std::make_pair(0, 0));
    _GameObjectManager->AddGameObject((GameObject*)camera);
    _GameObjectManager->AddGameObject((GameObject*)player);
    _GameObjectManager->AddGameObject(newDefaultSquare);
    _Window->SetCamera(camera);
    SetDefaultSquare(newDefaultSquare);

    // Adds in all special tiles
    for (auto i = specialTileList.begin(); i != specialTileList.end(); i++)
    {
        GameObject* newObject = NULL;           // Game object pointer to store newly made objects

        // Checks what character marked the special tile
        switch (i->first)
        {
        case 'd':
            newObject = new DestructibleWall(NULL, 0, destructibleWallTexture, { 0.4f, 0.075f, 0.0f, 1.0f }, i->second);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Destructible, newObject);
            break;
            // Generic coin
        case 'c':
            newObject = new Coin(coinTexture, { 1.0f, 1.0f, 1.0f, 1.0f }, i->second);
            newObject->SetScale(glm::vec2(1.25, 1.25));
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Coin, newObject);
            break;
        case 'C':
            newObject = new BigCoin(coinTexture, { 1.0f, 1.0f, 1.0f, 1.0f }, i->second);
            newObject->SetScale({ 3.5f, 3.5f });
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::BigCoin, newObject);
            _MapMatrix->SetTile(i->second.first + 1, i->second.second, MapMatrix::TileStatus::BigCoin, newObject);
            _MapMatrix->SetTile(i->second.first + 1, i->second.second + 1, MapMatrix::TileStatus::BigCoin, newObject);
            _MapMatrix->SetTile(i->second.first, i->second.second + 1, MapMatrix::TileStatus::BigCoin, newObject);
        default:
            std::cout << "Extra special tile found. Char: " << i->first << std::endl;
            break;
        }

        // Adds the new object to the object manager
        if (newObject)
        {
            _GameObjectManager->AddGameObject(newObject);
        }
    }

    // Updates the player position for the map
    _MapMatrix->UpdatePlayerPosition(player);
}

/*************************************************************************************************/
/*!
    \brief
        Loads in all the objects of the scene
*/
/*************************************************************************************************/
void Level1::UnloadScene()
{
    // Resets the default square
    SetDefaultSquare(NULL);

    // Shuts down those systems to refresh them
    _GameObjectManager->Shutdown();
    _TextureManager->Shutdown();
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
