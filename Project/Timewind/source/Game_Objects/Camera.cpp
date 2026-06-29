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
#include "../Engine/GameStateManager.h"
#include "../Visuals/CameraMovement.h"

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
	perspectiveChanged(true), viewMat(glm::mat4(0.0f)), perspMat(glm::mat4(0.0f)),
	upVector(glm::vec3(0.0f, 1.0f, 0.0f)), aspectRatio(aspectRatio_), fov(fieldOfView),
	centeredObject(centeredObject_), cameraBoxPos(pos), zDist(15.0f),
	lookAtOffset({ 0.0f }), cameraOffset({ 0.0f }), usePerspective(0.0f),
	cameraBoxRight(0.5f), cameraBoxLeft(0.5f), cameraBoxUp(4.0f), cameraBoxDown(1.0f), justGrounded(false),
	maxOffsetDistance(6.0f), useOffset(true), cameraSensitivity(4.0f),
	currentCameraMovement(NULL)
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
	// Checks if we are in normal gameplay
	if (_GameStateManager->GetGameState() <= GameStateManager::GameStates::Running)
	{
		// Updates the camera box in case the player has moved
		UpdateCameraBox(dt);

		// Checks if the player can move the camera
		if (_GameStateManager->GetGameState() == GameStateManager::GameStates::Walking || _GameStateManager->GetGameState() == GameStateManager::GameStates::Placing)
		{
			// Updates camera view positioning
			UpdatePlayerCameraMovements();

			// Updates the camera's variables
			zDist = 15.5f;
			usePerspective = 0.0f;
		}
		// Checks if the player is using the running camera
		else if (_GameStateManager->GetGameState() == GameStateManager::GameStates::Running)
		{
			// Sets the camera based on what direction the player is holding
			if (_InputManager->ReadInput(InputManager::Inputs::Right))
			{
				if (cameraOffset.x > 0.0f)
				{
					cameraOffset.x = cameraOffset.x - 16.0f * dt;
					lookAtOffset.x = lookAtOffset.x + 4.0f * dt;
				}
				else
				{
					cameraOffset.x = std::max(cameraOffset.x - (16.0f + cameraOffset.x * 1.5f) * dt, -8.0);
					lookAtOffset.x = std::min(lookAtOffset.x + (4.0f - lookAtOffset.x * 1.5f) * dt, 2.0);
				}
			}
			else if (_InputManager->ReadInput(InputManager::Inputs::Left))
			{
				if (cameraOffset.x < 0.0f)
				{
					cameraOffset.x = cameraOffset.x + 16.0f * dt;
					lookAtOffset.x = lookAtOffset.x - 4.0f * dt;
				}
				else
				{
					cameraOffset.x = std::min(cameraOffset.x + (16.0f - cameraOffset.x * 1.5f) * dt, 8.0);
					lookAtOffset.x = std::max(lookAtOffset.x - (4.0f + lookAtOffset.x * 1.5f) * dt, -2.0);
				}
			}
			else
			{
				cameraOffset /= 1.0f + 0.5f * dt;
			}

			// Updates the camera's variables
			zDist = 15.5f;
			cameraOffset.z = -(cameraOffset.x * cameraOffset.x / 12.0f);
			usePerspective = 1.0f;
		}
	}
	else if (_GameStateManager->GetGameState() == GameStateManager::GameStates::Cutscene)
	{
		// Checks that there is an unfinished camera movement
		if (currentCameraMovement)
		{
			if (currentCameraMovement->GetRemainingTime() > 0.0)
			{
				// Updates the camera movement
				currentCameraMovement->Update(dt);

				// Updates offsets in accordance
				lookAtOffset = currentCameraMovement->GetLookAtOffset();
				cameraOffset = currentCameraMovement->GetTranslationOffset();
			}
			else
			{
				// If the movement has finished, clears the camera movement
				currentCameraMovement = NULL;
			}
		}

		// Updates the camera's variables
		usePerspective = 1.0f;

		FixCameraPosition();
		std::cout << Get3DPosition().x << ", " << Get3DPosition().y << ", " << Get3DPosition().z << std::endl;
	}

	// Corrects the camera position given all offset changes made this frame
	FixCameraPosition();
}

/*************************************************************************************************/
/*!
	\brief
		Resets the camera offsets to (0, 0)
*/
/*************************************************************************************************/
void Camera::ResetCameraOffset()
{
	lookAtOffset = { 0.0f, 0.0f, 0.0f };
	cameraOffset = { 0.0f, 0.0f, 0.0f };
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
	// Update and return the view matrix
	viewMat = glm::lookAt(glm::vec3(Get3DPosition()), glm::vec3(cameraBoxPos.x + lookAtOffset.x, cameraBoxPos.y + lookAtOffset.y, lookAtOffset.z), upVector);
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
		// Calculates the new perspective and orthogonal matrix
		glm::mat4 perspective = glm::perspective(fov, aspectRatio, 0.1f, 100.0f);
		glm::mat4 orthogonal = glm::ortho(-16.0f, 16.0f, -16.0f / aspectRatio, 16.0f / aspectRatio, -1000.0f, 1000.0f);

		// Combines the two matrices
		perspMat = (perspective * usePerspective) + (orthogonal * (1.0f - usePerspective));

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
	FixCameraPosition();
	return glm::vec4(GetLookAtPosition() - GetPosition(), -zDist, 1.0f);
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
	FixCameraPosition();
	return glm::vec4(GetPosition(), -zDist + cameraOffset.z, 1.0f);
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
		glm::vec3 lookAtVector = glm::normalize(glm::vec3(cameraBoxPos.x + lookAtOffset.x, cameraBoxPos.y + lookAtOffset.y, lookAtOffset.z) - glm::vec3(GetPosition(), -zDist));

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
void Camera::SetCenteredObject(GameObject* object)
{
	centeredObject = object;
	cameraBoxPos = centeredObject->GetPosition();
	FixCameraPosition();
}

/*************************************************************************************************/
/*!
	\brief
		Starts the given camera movement

	\param newCameraMovement
		The new camera movement
*/
/*************************************************************************************************/
void Camera::StartCameraMovement(CameraMovement* newCameraMovement)
{
	currentCameraMovement = newCameraMovement;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Updates the camera position given all offsets
*/
/*************************************************************************************************/
void Camera::FixCameraPosition()
{
	// Calculates the x,y pos using all offsets
	SetPosition({ cameraBoxPos.x + cameraOffset.x, cameraBoxPos.y + cameraOffset.y });
}

/*************************************************************************************************/
/*!
	\brief
		Update function to move the camera offsets in tune with player inputs
*/
/*************************************************************************************************/
void Camera::UpdatePlayerCameraMovements()
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
	lookAtOffset.x = mouseDelta.x;
	lookAtOffset.y = mouseDelta.y;
	cameraOffset.x = mouseDelta.x;
	cameraOffset.y = mouseDelta.y;
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
	glm::vec2 coVel = ((Player*)centeredObject)->GetVelocity();

	// Checks if player has gone too far to the right
	if (coPos.x - cameraBoxPos.x > cameraBoxRight)
	{
		cameraBoxPos.x = coPos.x - cameraBoxRight;
	}
	else if (coPos.x - cameraBoxPos.x > 0.0f && coVel.x > 4.0f)
	{
		cameraBoxPos.x += coVel.x * (float)dt * 0.5f;
	}
	// Checks if the player has gone too far left
	else if (cameraBoxPos.x - coPos.x > cameraBoxLeft)
	{
		cameraBoxPos.x = coPos.x + cameraBoxLeft;
	}
	else if (cameraBoxPos.x - coPos.x > 0.0f && coVel.x < -4.0f)
	{
		cameraBoxPos.x += coVel.x * (float)dt * 0.5f;
	}

	if (!((Player*)centeredObject)->GetIsGrounded())
	{
		justGrounded = false;
	}
	else if (justGrounded == false && ((Player*)centeredObject)->GetIsGrounded())
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
