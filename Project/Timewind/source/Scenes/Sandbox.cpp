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

    // If the systems were found

    // Creates textures
    Texture* playerTexture = textureManager->AddTexture("Assets/Sprites/Alice_Neutral.png");
    Texture* wallTexture = textureManager->AddTexture("Assets/Sprites/Wall2.png");
    Texture* enemyTexture = textureManager->AddTexture("Assets/Sprites/Shadow_Enemy.png");
    Texture* destructibleWallTexture = textureManager->AddTexture("Assets/Sprites/Wall_Cracked.png");

    // Sets the default wall for the scene
    mapMatrix->SetDefaultWallTexture(wallTexture, { 0.4f, 0.075f, 0.0f, 1.0f });

    // Creates essential game objects (player and camera)
    Player* player = new Player({ 4.0f, 4.0f }, 0.0f, { 2.0f, 2.0f }, 50, playerTexture, mapMatrix, { 2, 2 });
    Camera* camera = new Camera(glm::vec2(4.0f, 4.5f), 0.0f, glm::vec2(0.0f, 0.0f), player, gameWindow->GetWindowSize().x / gameWindow->GetWindowSize().y, glm::radians(60.0f));
    objectManager->AddGameObject((GameObject*)camera);
    objectManager->AddGameObject((GameObject*)player);
    gameWindow->SetCamera(camera);

    // Creates scene game objects
    GameObject* destructibleWall = new GameObject({ 12.0f, 6.0f }, 0.0f, { 2.0f, 2.0f }, 0, true, destructibleWallTexture, true);
    destructibleWall->SetColor({ 0.4f, 0.075f, 0.0f, 1.0f });
    objectManager->AddGameObject(destructibleWall);
    Enemy* enemy = new Enemy({ 20.0f, 4.0f }, 0.0f, { 2.0f, 2.0f }, 40, enemyTexture, mapMatrix, { 10, 2 });
    objectManager->AddGameObject(enemy);

    // Sets the map for the scene
    mapMatrix->SetTile(10, 2, MapMatrix::TileStatus::Enemy, enemy);
    for (int i = 0; i < mapMatrix->GetMaxMapWidth(); i++)
    {
        mapMatrix->SetTile(i, 0, MapMatrix::TileStatus::Wall);
        mapMatrix->SetTile(i, 1, MapMatrix::TileStatus::Wall);
    }
    mapMatrix->SetTile(5, 2, MapMatrix::TileStatus::Wall);
    mapMatrix->SetTile(7, 3, MapMatrix::TileStatus::Wall);
    mapMatrix->SetTile(9, 4, MapMatrix::TileStatus::Wall);
    mapMatrix->SetTile(10, 4, MapMatrix::TileStatus::Wall);
    mapMatrix->SetTile(12, 5, MapMatrix::TileStatus::Wall);
    mapMatrix->SetTile(13, 5, MapMatrix::TileStatus::Wall);
    mapMatrix->SetTile(16, 5, MapMatrix::TileStatus::Wall);
    mapMatrix->SetTile(17, 5, MapMatrix::TileStatus::Wall);
    mapMatrix->SetTile(6, 3, MapMatrix::TileStatus::Destructible, destructibleWall);
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
