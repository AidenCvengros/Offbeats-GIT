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

// Base includes
#include "Camera.h"
#include "../Engine/cppShortcuts.h"

// Algorithm for the max and min functions
#include <algorithm>

// Includes the input manager to collect camera control inputs
#include "../Engine/InputManager.h"
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
	zDist(15.0f),
	cameraBoxPos(pos),
	cameraBoxLeft(0.5f), cameraBoxRight(0.5f), cameraBoxUp(4.0f), cameraBoxDown(1.0f),
	justGrounded(false),
	viewMat(glm::mat4(0.0f)),
	perspMat(glm::mat4(0.0f)),
	aspectRatio(aspectRatio_),
	fov(fieldOfView),
	lookAtOffset(0.0f, 0.0f), maxOffsetDistance(6.0f), useOffset(true), cameraSensitivity(4.0f)
{
	// If there is a centered object, starts the camera centered on that object
	if (centeredObject)
	{
		SetPosition(centeredObject->GetPosition());
		cameraBoxPos = centeredObject->CalculateRelativePositions(GameObject::Positions::Center);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Updates the game object.

	\param dt
		The time elapsed since the previous frame
*/
/*************************************************************************************************/
void Camera::Update(double dt)
{
	// Checks if the player can move the camera
	if (centeredObject->GetPlayerState() == Player::PlayerStates::Walking || centeredObject->GetPlayerState() == Player::PlayerStates::Placing)
	{
		// Updates camera view positioning
		UpdateRelativePosition();
		UpdateCameraBox(dt);

		// Updates the camera's position
		SetPosition(glm::vec2(cameraBoxPos.x, cameraBoxPos.y));
		zDist = 15.5f;
	}
	// Checks if the player is using the running camera
	else if (centeredObject->GetPlayerState() == Player::PlayerStates::Running)
	{
		// Updates the Camera Box
		UpdateCameraBox(dt);

		// Sets the camera behind the player
		float velocityOffset = centeredObject->GetVelocity().first;
		if (abs(velocityOffset) > 0.5f)
		{
			SetPosition(glm::vec2(cameraBoxPos.x - (velocityOffset / 2.0f), cameraBoxPos.y - (abs(velocityOffset) / 10.0f)));
			zDist = 15.5f - (velocityOffset * velocityOffset / 100.0f);
			lookAtOffset.x = (cameraBoxPos.x - GetPosition().x) / 5.0f;
		}
		// If the player is almost stopped, smooths the camera back to start
		else
		{
			MoveTo(cameraBoxPos, 0.5f, 0.0f);

			// Updates movements if any are active
			if (GetMoving())
			{
				MoveToUpdate(dt);
			}

			// Calculates the distance from cameraboxpos
			zDist = 15.5f - (15.5f - zDist) / (1.0f + dt);
			lookAtOffset.x = lookAtOffset.x * (1.0f - 0.9 * dt);
		}
	}
}

/*************************************************************************************************/
/*!
	\brief
		Resets the camera offset to (0, 0)
*/
/*************************************************************************************************/
void Camera::ResetCameraOffset()
{
	lookAtOffset = { 0.0f, 0.0f };
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
		// Update and return the view matrix
		//float distanceFromPlayer = glm::distance(cameraBoxPos, GetPosition());
		//float zCoord = -sqrt(abs(zDist * zDist - distanceFromPlayer * distanceFromPlayer));
		viewMat = glm::lookAt(glm::vec3(GetPosition(), -zDist), glm::vec3(cameraBoxPos.x + lookAtOffset.x, cameraBoxPos.y + lookAtOffset.y, 0.0), upVector);
	}
	else
	{
		// If there's not a centered object, default to (0, 0, 0)
		viewMat = glm::lookAt(glm::vec3(GetPosition(), -zDist), glm::vec3(0.0f, 0.0f, 0.0f), upVector);
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
		if (centeredObject->GetPlayerState() == Player::PlayerStates::Running)
		{
			// Calculates the new perspective matrix
			perspMat = glm::perspective(fov, aspectRatio, 0.1f, 100.0f);
		}
		else
		{
			// Calculates the orthogonal matrix
			perspMat = glm::ortho(-10.0f, 10.0f, -10.0f / aspectRatio, 10.0f / aspectRatio, -1000.0f, 1000.0f);
		}

		// Flips the screen upside down
		perspMat[1][1] *= -1.0;
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
	return glm::vec4(cameraBoxPos - GetPosition(), -zDist, 1.0f);
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
		Returns the point on the map the player is looking at

	\return
		The point the player is looking at
*/
/*************************************************************************************************/
glm::vec2 Camera::GetLookAtPosition()
{
	// Checks that there's a player
	if (centeredObject)
	{
		// Calculates the vector that the camera is looking at
		//float distanceFromPlayer = glm::distance(cameraBoxPos, GetPosition());
		//float zCoord = -sqrt(abs(zDist * zDist - distanceFromPlayer * distanceFromPlayer));
		glm::vec3 lookAtVector = glm::normalize(glm::vec3(cameraBoxPos.x + lookAtOffset.x, cameraBoxPos.y + lookAtOffset.y, 0.0f) - glm::vec3(GetPosition(), -zDist));

		// Calculates and returns what point in world space the camera is pointed at
		return (-glm::vec2(lookAtVector.x, lookAtVector.y) * (-zDist / lookAtVector.z)) + GetPosition();
	}
	else
	{
		// Otherwise returns the origin point
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
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
	cameraBoxPos = centeredObject->GetPosition();
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
	// Centers the camera when asked
	if (_InputManager->CheckInputStatus(InputManager::Inputs::CenterCamera) == InputManager::InputStatus::Pressed)
	{
		ResetCameraOffset();
	}

	// Gets the mouse inputs and adds in current mouse positioning
	glm::vec2 mouseDelta;
	mouseDelta.x = lookAtOffset.x - (cameraSensitivity * _InputManager->CheckMouseDelta().first / _Window->GetWindowSize().x);
	mouseDelta.y = lookAtOffset.y + (cameraSensitivity * _InputManager->CheckMouseDelta().second / _Window->GetWindowSize().y);

	// Checks if the delta is past the range
	if (glm::length(mouseDelta) > maxOffsetDistance)
	{
		// Caps the delta at the max distance
		mouseDelta = glm::normalize(mouseDelta) * maxOffsetDistance;
	}

	// Sets the look at offset
	lookAtOffset = mouseDelta;
}

/*************************************************************************************************/
/*!
	\brief
		Updates the camera box position

	\param dt
		The time elapsed since the previous frame
*/
/*************************************************************************************************/
void Camera::UpdateCameraBox(double dt)
{
	// Gets the centered object's position
	glm::vec2 coPos = centeredObject->CalculateRelativePositions(GameObject::Positions::Center);
	std::pair<float, float> coVel = centeredObject->GetVelocity();

	// Checks if player has gone too far to the right
	if (coPos.x - cameraBoxPos.x > cameraBoxRight)
	{
		cameraBoxPos.x = coPos.x - cameraBoxRight;
	}
	else if (coPos.x - cameraBoxPos.x > 0.0f && coVel.first > 4.0f)
	{
		cameraBoxPos.x += coVel.first * (float)dt * 0.5f;
	}
	// Checks if the player has gone too far left
	else if (cameraBoxPos.x - coPos.x > cameraBoxLeft)
	{
		cameraBoxPos.x = coPos.x + cameraBoxLeft;
	}
	else if (cameraBoxPos.x - coPos.x > 0.0f && coVel.first < -4.0f)
	{
		cameraBoxPos.x += coVel.first * (float)dt * 0.5f;
	}

	if (!centeredObject->GetIsGrounded())
	{
		justGrounded = false;
	}
	else if (justGrounded == false && centeredObject->GetIsGrounded())
	{
		cameraBoxPos.y += (coPos.y - cameraBoxPos.y) * 8.0f * (float)dt;

		if (abs(coPos.y - cameraBoxPos.y) < 0.0625f)
		{
			justGrounded = true;
			cameraBoxPos.y = coPos.y;
		}
	}

	// Checks if the player has gone too far up
	if (coPos.y - cameraBoxPos.y > cameraBoxUp)
	{
		cameraBoxPos.y = coPos.y - cameraBoxUp;
	}
	// Checks if the player has gone too far down
	else if (cameraBoxPos.y - coPos.y > cameraBoxDown)
	{
		cameraBoxPos.y = coPos.y + cameraBoxDown;
	}
}
