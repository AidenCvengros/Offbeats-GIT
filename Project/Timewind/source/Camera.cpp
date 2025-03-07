/*************************************************************************************************/
/*!
\file Camera.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.2.9
\brief
    FILL

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "Camera.h"

//-------------------------------------------------------------------------------------------------
// Private Constants
//-------------------------------------------------------------------------------------------------

const int zDist = -7.0f;						// How far back the camera is in the z axis

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
		Updates the game object.

	\param dt
		The time elapsed since the previous frame

	\param inputManager
		The input manager
*/
/*************************************************************************************************/
void Camera::Update(double dt, InputManager* inputManager)
{
	// Updates movements if any are active
	if (GetMoving())
	{
		MoveToUpdate(dt);
	}

	// Calculates placement based on which way the player is facing
	float directionModifier;
	if (centeredObject->GetIsFacingRight())
	{
		directionModifier = -1;
		//if (GetPosition().x < centeredObject->GetPosition().x - 4.0f)
		//{
		//	SetPosition(glm::vec2((centeredObject->GetPosition().x - 4.0f), -centeredObject->GetPosition().y));
		//}
		//else
		//{
		//	MoveTo(glm::vec2((centeredObject->GetPosition().x - 4.0f), -centeredObject->GetPosition().y), 0.5, false);
		//}
	}
	else
	{
		directionModifier = 1;
		//if (GetPosition().x > centeredObject->GetPosition().x + 4.0f)
		//{
		//	SetPosition(glm::vec2((centeredObject->GetPosition().x + 4.0f), -centeredObject->GetPosition().y));
		//}
		//else
		//{
		//	MoveTo(glm::vec2((centeredObject->GetPosition().x + 4.0f), -centeredObject->GetPosition().y), 0.5, false);
		//}
	}

	MoveTo(glm::vec2((centeredObject->GetPosition().x + (4.0f * directionModifier)), -centeredObject->GetPosition().y), 0.375, false);
}

/*************************************************************************************************/
/*!
	\brief
		Returns the view matrix of the camera

	\return
		The view matrix
*/
/*************************************************************************************************/
glm::mat4 Camera::GetViewMatrix()
{
	// If the rotation of the camera has changed since the last check
	if (centeredObject)
	{
		// Calculates whether the camera is facing left or right
		float directionModifier;
		if (centeredObject->GetIsFacingRight())
		{
			directionModifier = 1;
		}
		else
		{
			directionModifier = -1;
		}

		// Update and return the view matrix
		viewMat = glm::lookAt(glm::vec3(GetPosition(), zDist), glm::vec3(centeredObject->GetPosition().x + (2.0f * directionModifier), -centeredObject->GetPosition().y, 0.0f), upVector);
	}
	else
	{
		// If there's not a centered object, default to (0, 0, 0)
		viewMat = glm::lookAt(glm::vec3(GetPosition(), zDist), glm::vec3(0.0f, 0.0f, 0.0f), upVector);
	}
	
	// Then return the view matrix
	return viewMat;
}

/*************************************************************************************************/
/*!
	\brief
		Returns the perspective matrix of the camera

	\return
		The perspective matrix
*/
/*************************************************************************************************/
glm::mat4 Camera::GetPerspectiveMatrix()
{
	// Checks that the perspective matrix has been adjusted
	if (perspectiveChanged)
	{
		// Calculates the new perspective matrix
		perspMat = glm::perspective(fov, aspectRatio, 0.1f, 100.0f);
		perspMat[1][1] *= -1.25;
	}

	// Then return the perspective matrix
	return perspMat;
}

/*************************************************************************************************/
/*!
	\brief
		Returns the vector between the camera and the player

	\return
		The look at vector towards the player
*/
/*************************************************************************************************/
glm::vec4 Camera::GetLookAtVector()
{
	return glm::vec4(centeredObject->GetPosition() - GetPosition(), -zDist, 1.0f);
}

/*************************************************************************************************/
/*!
	\brief
		Returns the 3-dimensional position of the camera

	\return
		The position of the camera
*/
/*************************************************************************************************/
glm::vec4 Camera::Get3DPosition()
{
	return glm::vec4(GetPosition(), zDist, 1.0f);
}

/*************************************************************************************************/
/*!
	\brief
		Sets a game object for the camera to center on

	\param object
		The new centered object
*/
/*************************************************************************************************/
void Camera::SetCenteredObject(Player* object)
{
	centeredObject = object;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
