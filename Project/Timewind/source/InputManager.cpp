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

#include "InputManager.h"
#include <iostream>
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
		timeSincePressed.push_back(600);
	}

	keybinds.insert(std::pair(Inputs::Left, GLFW_KEY_LEFT));
	keybinds.insert(std::pair(Inputs::Right, GLFW_KEY_RIGHT));
	keybinds.insert(std::pair(Inputs::Up, GLFW_KEY_UP));
	keybinds.insert(std::pair(Inputs::Down, GLFW_KEY_DOWN));
	keybinds.insert(std::pair(Inputs::Jump, GLFW_KEY_SPACE));
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
		if (timeSincePressed[i] < 10.0)
		{
			timeSincePressed[i] += dt;
		}

		// Updates the input status for this key
		UpdateInputStatus((Inputs)i);
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
	if (timeSincePressed[(int)input] < 0.15)
	{
		// Returns that the button was pressed
		return InputStatus::Pressed;
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
		// Checks a keybind
		keyValue = glfwGetKey(window->GetWindowPtr(), i->second);

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
			timeSincePressed[(int)input] = 0.0;
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
