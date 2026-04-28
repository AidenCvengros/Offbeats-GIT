/*************************************************************************************************/
/*!
\file Main_Menu.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.4.27
\brief
    The main menu

    Functions include:
        + [FILL]

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Main_Menu.h"
#include "../Engine/cppShortcuts.h"

// Includes game object manager and texture manager for creating new game objects in the scene
#include "../Engine/GameObjectManager.h"
#include "../Engine/TextureManager.h"
#include "../Game_Objects/Player.h"
#include "../Game_Objects/Camera.h"
#include "../Engine/Font.h"
#include "../Game_Objects/Text.h"
#include "../Gameplay/Menu.h"
#include "../Gameplay/MenuOptions/GoToSceneOption.h"

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
void MainMenu::LoadScene()
{
    // Creates textures
    Texture* playerTexture = _TextureManager->AddTexture("Assets/Sprites/Alice_Neutral.png");
    Texture* wallTexture = _TextureManager->AddTexture("Assets/Sprites/Wall2.png");

    // Sets the default wall for the scene
    _MapMatrix->SetDefaultWallTexture(wallTexture, { 0.4f, 0.075f, 0.0f, 1.0f });

    // Sets the map for the scene
    std::vector< std::pair< char, std::pair< int, int > > > specialTileList;
    _MapMatrix->ReadMapFromFile("Assets/Maps/MainMenuBackground.csv", specialTileList);

    // Creates essential game objects (player, camera, and default square)
    Player* player = new Player({ 6.0f, 4.0f }, 0.0f, { 2.0f, 2.0f }, 50, playerTexture, { 3, 2 });
    Camera* camera = new Camera(glm::vec2(4.0f, 4.5f), 0.0f, glm::vec2(1.0f, 1.0f), player, _Window->GetWindowSize().x / _Window->GetWindowSize().y, glm::radians(50.0f));
    GameObject* newDefaultSquare = new GameObject({ 0.0f, 0.0f }, 0.0f, { 2.0f, 2.0f }, 0, true, { 1.0f, 1.0f, 1.0f, 1.0f }, std::make_pair(0, 0));
    _GameObjectManager->AddGameObject(camera);
    _GameObjectManager->AddGameObject(player);
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

    // Creates the main main menu menu
    mainMenu = new Menu();
    Text* startOptionText = new Text("Start", _TextureManager->GetDefaultFont(), 12, {8.0f, 6.0f}, 0.0f, {0.1f, 0.1f}, 90, {1.0f, 1.0f, 1.0f, 1.0f});
    Text* quitOptionText = new Text("Quit", _TextureManager->GetDefaultFont(), 12, { 8.0f, 5.0f }, 0.0f, { 0.1f, 0.1f }, 90, { 1.0f, 1.0f, 1.0f, 1.0f });
    GoToSceneOption* startOption = new GoToSceneOption(startOptionText, 101);
    GoToSceneOption* quitOption = new GoToSceneOption(quitOptionText, -1);
    _GameObjectManager->AddGameObject(startOptionText);
    _GameObjectManager->AddGameObject(quitOptionText);
    mainMenu->AddOption(startOption);
    mainMenu->AddOption(quitOption);
}

/*************************************************************************************************/
/*!
    \brief
        Updates the scene

    \param dt
        Time elapsed since previous frame
*/
/*************************************************************************************************/
void MainMenu::Update(double dt)
{
    // Checks for menu inputs
    if (_InputManager->CheckInputStatus({ InputManager::Inputs::Down, InputManager::Inputs::MenuSelect }) == InputManager::InputStatus::Pressed)
    {
        mainMenu->IncrementOptionIndex();
    }
    else if (_InputManager->CheckInputStatus(InputManager::Inputs::Up) == InputManager::InputStatus::Pressed)
    {
        mainMenu->DecrementOptionIndex();
    }
    
    // Checks for a selection
    if (_InputManager->CheckInputStatus(InputManager::Inputs::MenuAdvance) == InputManager::InputStatus::Pressed)
    {
        mainMenu->GetSelectedOption()->Selected();
    }
    else
    {
        // Otherwise runs hovering behavior
        mainMenu->GetSelectedOption()->Hovering();
    }
}

/*************************************************************************************************/
/*!
    \brief
        Loads in all the objects of the scene
*/
/*************************************************************************************************/
void MainMenu::UnloadScene()
{
    // Deletes the main main menu menu object
    delete mainMenu;

    // Resets the default square
    SetDefaultSquare(NULL);

    // Shuts down those systems to refresh them
    _GameObjectManager->Shutdown();
    _TextureManager->Clear();
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
