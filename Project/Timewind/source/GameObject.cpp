/*************************************************************************************************/
/*!
\file GameObject.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2023.11.28
\brief
    Defines the game object class for every game object in the game world

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "GameObject.h"

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
		Updates the game object. Can be overwritten by derived classes

	\param dt
		The time elapsed since the previous frame

	\param inputManager
		Allows the game objects to check inputs
*/
/*************************************************************************************************/
void GameObject::Update(double dt, InputManager* inputManager)
{
	// Updates movements if any are active
	if (moving)
	{
		MoveToUpdate(dt);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Returns the transformation matrix for the game object

	\return
		The game object's transformation matrix
*/
/*************************************************************************************************/
glm::mat4x4 GameObject::GetTranformationMatrix()
{
	return glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::translate(glm::mat4(1.0f), glm::vec3(position / 2.0f, 0.0f));
}

/*************************************************************************************************/
/*!
	\brief
		Returns the position of the game object

	\return
		The game object's position
*/
/*************************************************************************************************/
glm::vec2 GameObject::GetPosition()
{
	return position;
}

/*************************************************************************************************/
/*!
	\brief
		Returns whether the game object is moving

	\return
		Whether the game object is moving
*/
/*************************************************************************************************/
bool GameObject::GetMoving()
{
	return moving;
}

/*************************************************************************************************/
/*!
	\brief
		Returns the object's draw position

	\return
		The object's draw position
*/
/*************************************************************************************************/
int GameObject::GetDrawPriority()
{
	return drawPriority;
}

/*************************************************************************************************/
/*!
	\brief
		Sets the position of the game object

	\param newPosition
		The game object's new position
*/
/*************************************************************************************************/
void GameObject::SetPosition(glm::vec2 newPosition)
{
	position = newPosition;
}

/*************************************************************************************************/
/*!
	\brief
		Sets the rotation of the game object

	\param newRotation
		The game object's new rotation
*/
/*************************************************************************************************/
void GameObject::SetRotation(float newRotation)
{
	rotation = newRotation;
}

/*************************************************************************************************/
/*!
	\brief
		Sets the scale of the game object

	\param newScale
		The game object's new scale
*/
/*************************************************************************************************/
void GameObject::SetScale(glm::vec2 newScale)
{
	scale = newScale;
}

/*************************************************************************************************/
/*!
	\brief
		Sets a new movement

	\param newPosition
		The game object's new position

	\param time
		How long it takes the object to arrive there

	\param smooth
		Whether to move linearly of smooth into/out of the motion
*/
/*************************************************************************************************/
void GameObject::MoveTo(glm::vec2 newPosition, double time, bool smooth)
{
	// Sets the variable for the new move
	moving = true;
	moveOriginalPosition = position;
	moveNewPosition = newPosition;
	moveTime = time;
	moveTimeLeft = time;
	moveSmooth = smooth;
}

/*************************************************************************************************/
/*!
	\brief
		Updates an object's active movement

	\param dt
		The time elapsed since the previous frame
*/
/*************************************************************************************************/
void GameObject::MoveToUpdate(double dt)
{
	// Decrements the time left by dt
	moveTimeLeft -= dt;

	// Checks that the movement is not done
	if (moveTimeLeft > 0.0)
	{
		// Moves the object
		SetPosition((moveNewPosition - moveOriginalPosition) * (float)(1 - (moveTimeLeft / moveTime)) + moveOriginalPosition);
	}
	// Otherwise sets the position to the final position and ends the movement
	else
	{
		SetPosition(moveNewPosition);
		moving = false;
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
