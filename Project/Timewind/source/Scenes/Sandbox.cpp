/*************************************************************************************************/
/*!
\file Sandbox.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.5.22
\brief
    The sandbox scene for quick testing

    Functions include:
        + [FILL]

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Sandbox.h"
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
void Sandbox::LoadScene()
{
    // Creates textures
    Texture* playerTexture = _TextureManager->AddTexture("Assets/Sprites/Alice_Neutral.png");
    Texture* wallTexture = _TextureManager->AddTexture("Assets/Sprites/Wall2.png");
    Texture* enemyTexture = _TextureManager->AddTexture("Assets/Sprites/Shadow_Enemy.png");
    Texture* destructibleWallTexture = _TextureManager->AddTexture("Assets/Sprites/Wall_Cracked.png");
    Texture* keyTexture = _TextureManager->AddTexture("Assets/Sprites/Key.png");
    Texture* lockedWallTexture = _TextureManager->AddTexture("Assets/Sprites/LockedWall.png");
    Texture* coinTexture = _TextureManager->AddTexture("Assets/Sprites/Coin.png");
    Texture* bumperTexture = _TextureManager->AddTexture("Assets/Sprites/Bumper.png");

    // Sets the default wall for the scene
    _MapMatrix->SetDefaultWallTexture(wallTexture, { 0.4f, 0.075f, 0.0f, 1.0f });

    // Creates essential game objects (player and camera)
    Player* player = new Player({ 4.0f, 4.0f }, 0.0f, { 2.0f, 2.0f }, 50, playerTexture, { 2, 2 });
    Camera* camera = new Camera(glm::vec2(4.0f, 4.5f), 0.0f, glm::vec2(0.0f, 0.0f), player, _Window->GetWindowSize().x / _Window->GetWindowSize().y, glm::radians(50.0f));
    _GameObjectManager->AddGameObject((GameObject*)camera);
    _GameObjectManager->AddGameObject((GameObject*)player);
    _Window->SetCamera(camera);

    // Sets the map for the scene
    std::vector< std::pair< char, std::pair< int, int > > > specialTileList;
    _MapMatrix->ReadMapFromFile("Assets/Maps/Sandbox.csv", specialTileList);

    // Adds in all special tiles
    for (auto i = specialTileList.begin(); i != specialTileList.end(); i++)
    {
        GameObject* newObject;                  // Game object pointer to store newly made objects

        // Checks what character marked the special tile
        switch (i->first)
        {
            // Key 1 is a key hidden behind a destructible wall
        case '1':
            newObject = new Key(33, keyTexture, { 0.859f, 0.255f, 0.380f, 1.0f }, i->second);
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Key, newObject);
            break;
            // Key 2 is a free key
        case '2':
            newObject = new Key(18, keyTexture, { 0.604f, 0.922f, 0.0f, 1.0f }, i->second);
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Key, newObject);
            break;
            // Key 3 is a free key
        case '3':
            newObject = new Key(13, keyTexture, { 0.286f, 0.667f, 0.063f, 1.0f }, i->second);
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Key, newObject);
            break;
            // Key 4 is hidden behind a destructible wall
        case '4':
            newObject = new Key(55, keyTexture, { 1.0f, 1.0f, 1.0f, 1.0f }, i->second);
            newObject = new DestructibleWall(newObject, 0, destructibleWallTexture, { 0.4f, 0.075f, 0.0f, 1.0f }, i->second);
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Destructible, newObject);
            break;
            // Key 5 is hidden behind a destructible wall
        case '5':
            newObject = new Key(6, keyTexture, { 0.380f, 0.827f, 0.890f, 1.0f }, i->second);
            newObject = new DestructibleWall(newObject, 0, destructibleWallTexture, { 0.4f, 0.075f, 0.0f, 1.0f }, i->second);
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Destructible, newObject);
            break;
            // Door ! is the lock for key 1
        case '!':
            newObject = new LockedWall(33, 40, lockedWallTexture, { 0.859f, 0.255f, 0.380f, 1.0f }, i->second);
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::LockedDoor, newObject);
            break;
            // Door @ is the lock for key 2
        case '@':
            newObject = new LockedWall(18, 40, lockedWallTexture, { 0.604f, 0.922f, 0.0f, 1.0f }, i->second);
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::LockedDoor, newObject);
            break;
            // Door # is the lock for key 3
        case '#':
            newObject = new LockedWall(13, 40, lockedWallTexture, { 0.286f, 0.667f, 0.063f, 1.0f }, i->second);
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::LockedDoor, newObject);
            break;
            // Door $ is the lock for key 4
        case '$':
            newObject = new LockedWall(55, 40, lockedWallTexture, { 1.0f, 1.0f, 1.0f, 1.0f }, i->second);
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::LockedDoor, newObject);
            break;
            // Door % is the lock for key 5
        case '%':
            newObject = new LockedWall(6, 40, lockedWallTexture, { 0.380f, 0.827f, 0.890f, 1.0f }, i->second);
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::LockedDoor, newObject);
            break;
            // Generic destructible block
        case 'd':
            newObject = new DestructibleWall(NULL, 0, destructibleWallTexture, { 0.4f, 0.075f, 0.0f, 1.0f }, i->second);
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Destructible, newObject);
            break;
            // Generic coin
        case 'c':
            newObject = new Coin(coinTexture, { 1.0f, 1.0f, 1.0f, 1.0f }, i->second);
            newObject->SetScale(glm::vec2(1.25, 1.25));
            _GameObjectManager->AddGameObject(newObject);
            _MapMatrix->SetTile(i->second, MapMatrix::TileStatus::Coin, newObject);
            break;
        default:
            std::cout << "Extra special tile found. Char: " << i->first << std::endl;
            break;
        }
    }

    ScreenCrack* glitchBox = new ScreenCrack(glm::vec2(11.0f, 10.5f), 40.0f, glm::vec2(3.0f, 3.0f), 150, true, glm::vec4(0.0f, 0.0f, 0.0f, 0.9f), 12.0f);
    _GameObjectManager->AddGameObject(glitchBox);

    Bumper* bumper = new Bumper(glm::vec2(8.0f, 4.0f), 180.0f, glm::vec2(2.0f, 2.0f), true, bumperTexture, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), std::make_pair(4, 2));
    _MapMatrix->SetTile(std::make_pair(4, 2), MapMatrix::TileStatus::Bumper, bumper);
    _GameObjectManager->AddGameObject(bumper);

    // Updates the player position for the map
    _MapMatrix->UpdatePlayerPosition(player);
}

/*************************************************************************************************/
/*!
    \brief
        Loads in all the objects of the scene
*/
/*************************************************************************************************/
void Sandbox::UnloadScene()
{
    // Shuts down those systems to refresh them
    _GameObjectManager->Shutdown();
    _TextureManager->Shutdown();
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
