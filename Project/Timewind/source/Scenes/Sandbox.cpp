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

#include "Sandbox.h"

#include "../cppShortcuts.h"

// Includes the Engine functionality
#include "../Engine.h"

// Includes game object manager and texture manager for creating new game objects in the scene
#include "../GameObjectManager.h"
#include "../TextureManager.h"
#include "../Game_Objects/Enemy.h"
#include "../Game_Objects/DestructibleWall.h"
#include "../Game_Objects/Key.h"
#include "../LockedWall.h"

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

    \param mapMatrix
        The map grid for the current scene
*/
/*************************************************************************************************/
void Sandbox::LoadScene(MapMatrix* mapMatrix)
{
    // Gets the systems that game objects are loaded with
    GameObjectManager* objectManager = _GameObjectManager;
    TextureManager* textureManager = _TextureManager;
    Window* gameWindow = (Window*)engine->GetSystem(Engine::SystemTypes::window);

    // Checks that the systems were returned properly
    if (!objectManager || !textureManager)
    {
        // Throws an error
        throw std::runtime_error("Engine system was not found for scene initialization.");
    }

    // Creates textures
    Texture* playerTexture = textureManager->AddTexture("Assets/Sprites/Alice_Neutral.png");
    Texture* wallTexture = textureManager->AddTexture("Assets/Sprites/Wall2.png");
    Texture* enemyTexture = textureManager->AddTexture("Assets/Sprites/Shadow_Enemy.png");
    Texture* destructibleWallTexture = textureManager->AddTexture("Assets/Sprites/Wall_Cracked.png");
    Texture* keyTexture = textureManager->AddTexture("Assets/Sprites/Key.png");
    Texture* lockedWallTexture = textureManager->AddTexture("Assets/Sprites/LockedWall.png");

    // Sets the default wall for the scene
    mapMatrix->SetDefaultWallTexture(wallTexture, { 0.4f, 0.075f, 0.0f, 1.0f });

    // Creates essential game objects (player and camera)
    Player* player = new Player({ 4.0f, 4.0f }, 0.0f, { 2.0f, 2.0f }, 50, playerTexture, mapMatrix, { 2, 2 });
    Camera* camera = new Camera(glm::vec2(4.0f, 4.5f), 0.0f, glm::vec2(0.0f, 0.0f), player, gameWindow->GetWindowSize().x / gameWindow->GetWindowSize().y, glm::radians(60.0f));
    objectManager->AddGameObject((GameObject*)camera);
    objectManager->AddGameObject((GameObject*)player);
    gameWindow->SetCamera(camera);

    // Sets the map for the scene
    std::vector< std::pair< char, std::pair< int, int > > > specialTileList;
    mapMatrix->ReadMapFromFile("Assets/Maps/Level1.csv", specialTileList);

    // Adds in all special tiles
    for (auto i = specialTileList.begin(); i != specialTileList.end(); i++)
    {
        GameObject* newObject;                  // Game object pointer to store newly made objects

        // Checks what character marked the special tile
        switch (i->first)
        {
        // Case 1 is a key hidden behind a destructible wall
        case '1':
            newObject = new Key(33, keyTexture, { 0.859, 0.255, 0.380, 1.0f }, i->second);
            newObject = new DestructibleWall(newObject, 0, destructibleWallTexture, { 0.4f, 0.075f, 0.0f, 1.0f }, i->second);
            objectManager->AddGameObject(newObject);
            mapMatrix->SetTile(i->second, MapMatrix::TileStatus::Destructible, newObject);
            break;
        // Case 2 is a free key
        case '2':
            newObject = new Key(18, keyTexture, { 0.604f, 0.922f, 0.0f, 1.0f }, i->second);
            objectManager->AddGameObject(newObject);
            mapMatrix->SetTile(i->second, MapMatrix::TileStatus::Key, newObject);
            break;
        // Case ! is the lock for key 1
        case '!':
            newObject = new LockedWall(33, 40, lockedWallTexture, { 0.859f, 0.255f, 0.380f, 1.0f }, i->second);
            objectManager->AddGameObject(newObject);
            mapMatrix->SetTile(i->second, MapMatrix::TileStatus::LockedDoor, newObject);
            break;
        // Case @ is the lock for key 2
        case '@':
            newObject = new LockedWall(18, 40, lockedWallTexture, { 0.859f, 0.255f, 0.380f, 1.0f }, i->second);
            objectManager->AddGameObject(newObject);
            mapMatrix->SetTile(i->second, MapMatrix::TileStatus::LockedDoor, newObject);
            break;
        default:
            std::cout << "Extra special tile found. Char: " << i->first << std::endl;
            break;
        }
    }

    // Creates scene game objects
    
    //objectManager->AddGameObject(key);
    
    //destructibleWall->SetColor({ 0.4f, 0.075f, 0.0f, 1.0f });
    
    //Enemy* enemy = new Enemy({ 20.0f, 4.0f }, 0.0f, { 2.0f, 2.0f }, 40, enemyTexture, mapMatrix, { 10, 2 });
    //objectManager->AddGameObject(enemy);
    

    
    mapMatrix->UpdatePlayerPosition(player);
    //mapMatrix->SetTile(10, 2, MapMatrix::TileStatus::Enemy, enemy);
    //for (int i = 0; i < mapMatrix->GetMaxMapWidth(); i++)
    //{
    //    mapMatrix->SetTile(i, 0, MapMatrix::TileStatus::Wall);
    //    mapMatrix->SetTile(i, 1, MapMatrix::TileStatus::Wall);
    //}
    //mapMatrix->SetTile(5, 2, MapMatrix::TileStatus::Wall);
    //mapMatrix->SetTile(7, 3, MapMatrix::TileStatus::Wall);
    //mapMatrix->SetTile(9, 4, MapMatrix::TileStatus::Wall);
    //mapMatrix->SetTile(10, 4, MapMatrix::TileStatus::Wall);
    //mapMatrix->SetTile(12, 5, MapMatrix::TileStatus::Wall);
    //mapMatrix->SetTile(13, 5, MapMatrix::TileStatus::Wall);
    //mapMatrix->SetTile(16, 5, MapMatrix::TileStatus::Wall);
    //mapMatrix->SetTile(17, 5, MapMatrix::TileStatus::Wall);
    //mapMatrix->SetTile(6, 3, MapMatrix::TileStatus::Destructible, destructibleWall);
    //mapMatrix->SetTile(15, 2, MapMatrix::TileStatus::LockedDoor, lockedWall);
    //mapMatrix->SetTile(6, 4, MapMatrix::TileStatus::Key, key);
}

/*************************************************************************************************/
/*!
    \brief
        Loads in all the objects of the scene
*/
/*************************************************************************************************/
void Sandbox::UnloadScene()
{
    // Gets the systems that game objects are loaded with
    GameObjectManager* objectManager = (GameObjectManager*)engine->GetSystem(Engine::SystemTypes::gameObjectManager);
    TextureManager* textureManager = (TextureManager*)engine->GetSystem(Engine::SystemTypes::textureManager);

    // Shuts down those systems to refresh them
    objectManager->Shutdown();
    textureManager->Shutdown();
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
