/*************************************************************************************************/
/*!
\file InputManager.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.2.16
\brief
    Gathers and manages inputs from the window instance

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "InputManager.h"
#include "cppShortcuts.h"

// Includes glfw input reading functionality
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Includes the window class that we are reading inputs from
#include "Window.h"

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
		Initializes the system.
*/
/*************************************************************************************************/
void InputManager::Init()
{
	// Puts in a blank value for each input category
	for (int i = 0; i < (size_t)Inputs::Max; i++)
	{
		inputTracker.push_back(InputStatus::Off);
		timeSincePressed.push_back({ 100, false });
	}

	keybinds.insert(std::pair(Inputs::Escape, GLFW_KEY_ESCAPE));
	keybinds.insert(std::pair(Inputs::Pause, GLFW_KEY_ESCAPE));
	keybinds.insert(std::pair(Inputs::TogglePlacing, GLFW_KEY_TAB));
	keybinds.insert(std::pair(Inputs::MenuAdvance, GLFW_KEY_E));
	keybinds.insert(std::pair(Inputs::MenuAdvance, GLFW_KEY_ENTER));
	keybinds.insert(std::pair(Inputs::MenuBack, GLFW_KEY_Q));
	keybinds.insert(std::pair(Inputs::MenuBack, GLFW_KEY_BACKSPACE));
	keybinds.insert(std::pair(Inputs::Left, GLFW_KEY_LEFT));
	keybinds.insert(std::pair(Inputs::Left, GLFW_KEY_A));
	keybinds.insert(std::pair(Inputs::Right, GLFW_KEY_RIGHT));
	keybinds.insert(std::pair(Inputs::Right, GLFW_KEY_D));
	keybinds.insert(std::pair(Inputs::Up, GLFW_KEY_UP));
	keybinds.insert(std::pair(Inputs::Up, GLFW_KEY_W));
	keybinds.insert(std::pair(Inputs::Down, GLFW_KEY_DOWN));
	keybinds.insert(std::pair(Inputs::Down, GLFW_KEY_S));
	keybinds.insert(std::pair(Inputs::MovementJump, GLFW_KEY_SPACE));
	keybinds.insert(std::pair(Inputs::MovementAction, GLFW_KEY_E));
	keybinds.insert(std::pair(Inputs::PlacementPlace, GLFW_MOUSE_BUTTON_LEFT));
	keybinds.insert(std::pair(Inputs::PlacementPickup, GLFW_MOUSE_BUTTON_RIGHT));
	keybinds.insert(std::pair(Inputs::PlacementRotateLeft, GLFW_KEY_Q));
	keybinds.insert(std::pair(Inputs::PlacementRotateRight, GLFW_KEY_E));
	keybinds.insert(std::pair(Inputs::F1, GLFW_KEY_F1));
	keybinds.insert(std::pair(Inputs::F2, GLFW_KEY_F2));
	keybinds.insert(std::pair(Inputs::F3, GLFW_KEY_F3));

	// Grabs the cursor and uses it's current position as the starting for the program
	glfwSetInputMode(_Window->GetVulkanWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(_Window->GetVulkanWindowPtr(), &mouseCoords.first, &mouseCoords.second);
}

/*************************************************************************************************/
/*!
	\brief
		Updates the system.

	\param
		The time elapsed since the previous frame.
*/
/*************************************************************************************************/
void InputManager::Update(double dt)
{
	// Updates each input category
	for (int i = 0; i < (size_t)Inputs::Max; i++)
	{
		// Increments the buffer tracker and ensures it doesn't overflow
		if (timeSincePressed[i].first < 10.0)
		{
			timeSincePressed[i].first += dt;
		}

		// Updates the input status for this key
		UpdateInputStatus((Inputs)i);
	}

	std::pair<double, double> oldMouseCoords = mouseCoords;
	glfwGetCursorPos(_Window->GetVulkanWindowPtr(), &mouseCoords.first, &mouseCoords.second);
	mouseDelta = { mouseCoords.first - oldMouseCoords.first, mouseCoords.second - oldMouseCoords.second };
	if (CheckInputStatus(Inputs::F3) == InputStatus::Pressed)
	{
		std::cout << mouseCoords.first << ", " << mouseCoords.second << std::endl;
	}

	if (CheckInputStatus(Inputs::Escape) == InputStatus::Pressed)
	{
		glfwSetWindowShouldClose(_Window->GetVulkanWindowPtr(), true);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Draws the system to the screen.
*/
/*************************************************************************************************/
void InputManager::Draw()
{

}

/*************************************************************************************************/
/*!
	\brief
		Shuts down the system.
*/
/*************************************************************************************************/
void InputManager::Shutdown()
{

}

/*************************************************************************************************/
/*!
	\brief
		Check the status of the given input

	\param input
		The given input

	\return
		The status of the input
*/
/*************************************************************************************************/
InputManager::InputStatus InputManager::CheckInputStatus(Inputs input)
{
	// If the buffer check passed
	if (timeSincePressed[(int)input].first < 0.05 && timeSincePressed[(int)input].second == true)
	{
		// Returns that the button was pressed
		timeSincePressed[(int)input].second = false;
		inputTracker[(int)input] = InputStatus::Pressed;
	}

	// Otherwise returns the button status
	return inputTracker[(int)input];
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Updates the given input status on the tracker

	\param input
		The given input
*/
/*************************************************************************************************/
void InputManager::UpdateInputStatus(Inputs input)
{
	// Fetches the current state of the key	
	int keyValue = 0;
	auto range = keybinds.equal_range(input);
	for (auto i = range.first; i != range.second; i++)
	{
		// Checks for a mouse button
		if (i->second < 32)
		{
			keyValue = glfwGetMouseButton(_Window->GetVulkanWindowPtr(), i->second);
		}
		// Checks for a keyboard button
		else
		{
			keyValue = glfwGetKey(_Window->GetVulkanWindowPtr(), i->second);
		}

		// If a valid keybind is being pressed, break the loop
		if (keyValue)
		{
			break;
		}
	}
	
	// Checks if the key is down
	if (keyValue == GLFW_PRESS)
	{
		// Checks if the key was previously off
		if (inputTracker[(int)input] == InputStatus::Off)
		{
			// Sets the key to pressed
			inputTracker[(int)input] = InputStatus::Pressed;
			timeSincePressed[(int)input].first = 0.0;
			timeSincePressed[(int)input].second = true;
		}
		// Checks if the key was previously pressed
		else if (inputTracker[(int)input] == InputStatus::Pressed)
		{
			// Sets the key to held
			inputTracker[(int)input] = InputStatus::Held;
		}
	}
	// Checks if the key is up
	else if (keyValue == GLFW_RELEASE)
	{
		// Checks if the key was previously pressed or held
		if (inputTracker[(int)input] == InputStatus::Pressed || inputTracker[(int)input] == InputStatus::Held)
		{
			// Sets the key to released
			inputTracker[(int)input] = InputStatus::Released;
		}
		// Checks if the key was previously released
		else if (inputTracker[(int)input] == InputStatus::Released)
		{
			// Sets the key to off
			inputTracker[(int)input] = InputStatus::Off;
		}
	}
}
