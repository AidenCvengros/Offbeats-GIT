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
    GameObjectManager* objectManager = (GameObjectManager*)engine->GetSystem(Engine::SystemTypes::gameObjectManager);
    TextureManager* textureManager = (TextureManager*)engine->GetSystem(Engine::SystemTypes::textureManager);
    Window* gameWindow = (Window*)engine->GetSystem(Engine::SystemTypes::window);

    // Checks that the systems were returned properly
    if (!objectManager || !textureManager)
    {
        // Throws an error
        throw std::runtime_error("Engine system was not found for scene initialization.");
    }

    // If the systems were found

    // Creates textures
    Texture* tempTexture = textureManager->AddTexture("Assets/Sprites/Ashe_Token.png");
    Texture* tempTexture2 = textureManager->AddTexture("Assets/Sprites/Grass.png");
    Texture* tempTexture3 = textureManager->AddTexture("Assets/Sprites/Shadow_Enemy.png");

    // Creates essential game objects (player and camera)
    Player* player = new Player({ 4.0f, 4.0f }, 0.0f, { 2.0f, 2.0f }, 50, tempTexture, mapMatrix, { 2, 2 });
    Camera* camera = new Camera(glm::vec2(4.0f, 4.5f), 0.0f, glm::vec2(0.0f, 0.0f), player, gameWindow->GetWindowSize().x / gameWindow->GetWindowSize().y, glm::radians(45.0f));
    objectManager->AddGameObject((GameObject*)camera);
    objectManager->AddGameObject((GameObject*)player);
    gameWindow->SetCamera(camera);

    // Creates scene game objects
    GameObject* object = new GameObject({ 4.0f, 4.0f }, 0.0f, { 10.0f, 10.0f }, 0, true, tempTexture2, true);
    objectManager->AddGameObject(object);
    Enemy* enemy = new Enemy({ 20.0f, 4.0f }, 0.0f, { 2.0f, 2.0f }, 40, tempTexture3, mapMatrix, { 10, 2 });
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
    mapMatrix->SetTile(4, 2, MapMatrix::TileStatus::Destructible);
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
