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

#include "cppShortcuts.h"

//#include "InputManager.h"

//-------------------------------------------------------------------------------------------------
// Private Constants
//-------------------------------------------------------------------------------------------------

const int zDist = -10.0f;						// How far back the camera is in the z axis

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
		Constructor for the camera class

	\param pos
		The position of the game object

	\param rot
		The rotation of the game object

	\param sca
		The scale of the game object

	\param centeredObject_
		The game object to center the camera on

	\param windowWidth_
		The width of the view space

	\param windowHeight_
		The height of the view space
*/
/*************************************************************************************************/
Camera::Camera(glm::vec2 pos, float rot, glm::vec2 sca, Player* centeredObject_, float aspectRatio_, float fieldOfView) :
	GameObject(pos, rot, sca),
	//rotationChanged(true),
	perspectiveChanged(true),
	inCutscene(false),
	upVector(glm::vec3(0.0f, 1.0f, 0.0f)),
	centeredObject(centeredObject_),
	viewMat(glm::mat4(0.0f)),
	perspMat(glm::mat4(0.0f)),
	aspectRatio(aspectRatio_),
	fov(fieldOfView),
	relativePosX(0.0), relativePosY(0.0)
{
	SetPosition(centeredObject->GetPosition());
}

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

	if (inCutscene == false)
	{
		// Updates camera view positioning
		UpdateRelativePosition();

		MoveTo(glm::vec2(centeredObject->GetPosition().x + 5.0f * relativePosX, centeredObject->GetPosition().y + -3.75f * relativePosY), 0.025, false);
	}
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
		float distanceFromPlayer = glm::distance(centeredObject->GetPosition(), GetPosition());
		viewMat = glm::lookAt(glm::vec3(GetPosition(), zDist + distanceFromPlayer * distanceFromPlayer / 8.0f), glm::vec3(centeredObject->GetPosition().x, centeredObject->GetPosition().y, zDist / 4.0f), upVector);
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
	SetPosition(centeredObject->GetPosition());
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Updates the relative camera position
*/
/*************************************************************************************************/
void Camera::UpdateRelativePosition()
{
	relativePosX = max(min(relativePosX + (_InputManager->CheckMouseDelta().first / 1000.0), 1.0), -1.0);
	relativePosY = max(min(relativePosY + (_InputManager->CheckMouseDelta().second / 1000.0), 1.0), -1.0);

	glm::vec2 normalizedRelativePositions = glm::normalize(glm::vec2{ relativePosX, relativePosY });

	if (abs(relativePosX) > abs(normalizedRelativePositions.x))
	{
		relativePosX = normalizedRelativePositions.x;
	}
	if (abs(relativePosY) > abs(normalizedRelativePositions.y))
	{
		relativePosY = normalizedRelativePositions.y;
	}
}
