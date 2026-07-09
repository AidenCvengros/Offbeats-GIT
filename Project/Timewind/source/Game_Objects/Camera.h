/*************************************************************************************************/
/*!
\file Camera.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.2.9
\brief
    Camera game object

    Public Functions:
        + FILL
		
	Private Functions:
		+ FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_Camera_H_
#define Syncopatience_Camera_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base include
#include "../Engine/stdafx.h"

// Base game object class
#include "GameObject.h"

// Player class for the camera to be centered on
#include "Player.h"

//-------------------------------------------------------------------------------------------------
// Forward References
//-------------------------------------------------------------------------------------------------

class CameraMovement;

//-------------------------------------------------------------------------------------------------
// Public Constants
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Classes
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		The camera game object class
*/
/*************************************************************************************************/
class Camera : public GameObject
{
public:
	//---------------------------------------------------------------------------------------------
	// Public Consts
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Structures
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Variables
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Function Declarations
	//---------------------------------------------------------------------------------------------
	
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
	Camera(glm::vec2 pos, float rot, glm::vec2 sca, Player* centeredObject_, float aspectRatio_, float fieldOfView);
	
	/*************************************************************************************************/
	/*!
		\brief
			Destructor for the camera class
	*/
	/*************************************************************************************************/
	~Camera() {}

	/*************************************************************************************************/
	/*!
		\brief
			Updates the game object.
	
		\param dt
			The time elapsed since the previous frame
	*/
	/*************************************************************************************************/
	void Update(double dt);

	/*************************************************************************************************/
	/*!
		\brief
			Resets the camera offsets to (0, 0)
	*/
	/*************************************************************************************************/
	void ResetCameraOffset();

	/*************************************************************************************************/
	/*!
		\brief
			Returns the view matrix of the camera

		\return
			The view matrix
	*/
	/*************************************************************************************************/
	glm::mat4 GetViewMatrix();

	/*************************************************************************************************/
	/*!
		\brief
			Returns the perspective matrix of the camera

		\return
			The perspective matrix
	*/
	/*************************************************************************************************/
	glm::mat4 GetPerspectiveMatrix();

	/*************************************************************************************************/
	/*!
		\brief
			Returns the vector between the camera and the player

		\return
			The look at vector towards the player
	*/
	/*************************************************************************************************/
	glm::vec4 GetLookAtVector();

	/*************************************************************************************************/
	/*!
		\brief
			Returns the 3-dimensional position of the camera

		\return
			The position of the camera
	*/
	/*************************************************************************************************/
	glm::vec4 Get3DPosition();

	/*************************************************************************************************/
	/*!
		\brief
			Returns the point on the map the player is looking at

		\return
			The point the player is looking at
	*/
	/*************************************************************************************************/
	glm::vec2 GetLookAtPosition();

	/*************************************************************************************************/
	/*!
		\brief
			Returns the point the camera is orientated around

		\return
			The point the player is looking at
	*/
	/*************************************************************************************************/
	glm::vec2 GetCameraBoxPosition() { return cameraBoxPos; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the aspect ratio of the game window

		\return
			The aspect ratio (height/width)
	*/
	/*************************************************************************************************/
	float GetAspectRatio() { return aspectRatio; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the aspect ratio of the game window

		\return
			The aspect ratio (height/width)
	*/
	/*************************************************************************************************/
	float GetPerspective() { return usePerspective; }

	/*************************************************************************************************/
	/*!
		\brief
			Sets a game object for the camera to center on

		\param object
			The new centered object
	*/
	/*************************************************************************************************/
	void SetCenteredObject(GameObject* object);

	/*************************************************************************************************/
	/*!
		\brief
			Starts the given camera movement

		\param newCameraMovement
			The new camera movement
	*/
	/*************************************************************************************************/
	void StartCameraMovement(CameraMovement* newCameraMovement);
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Structures
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Variables
	//---------------------------------------------------------------------------------------------
	
	// Basic camera operation variables
	bool perspectiveChanged;					// Boolean for updating camera perspective matrix
	glm::mat4 viewMat;							// The view matrix for the camera
	glm::mat4 perspMat;							// The perspective matrix for the camera
	glm::vec3 upVector;							// The up vector for the camera
	float aspectRatio;							// The aspect ratio of the camera view
	float fov;									// The field of view of the camera view

	// By default the camera has an object confined within a camera box and the camera follows the camera box at zDist away.
	// This can be changed in two ways:
	// - The look at offset moves the point the camera is looking at. This offset is in reference to (cameraBoxPos.x, cameraBoxPos.y, 0.0).
	// - The camera offset moves where the camera eye is positioned. This offset is in reference to (cameraBoxPos.x, cameraBoxPos.y, zDist).
	GameObject* centeredObject;					// The game object that the camera is focusing on (probably the player)
	bool centeredObjectIsPlayer;				// Tracks whether the centered object is the player
	glm::vec2 cameraBoxPos;						// The position of the camera box
	float zDist;								// How far in the z direction the camera is
	glm::vec3 lookAtOffset;						// How far to the side/up the camera is looking
	glm::vec3 cameraOffset;						// How far to the side/up from the camera box position that camera is
	float usePerspective;						// How much perspective vs orthogonal to use (0 is orthogonal, 1 is perspective)

	// Variables for calculating default camera box position
	float cameraBoxRight;						// How far right the camera box extends
	float cameraBoxLeft;						// How far left the camera box extends
	float cameraBoxUp;							// How far up the camera box extends
	float cameraBoxDown;						// How far down the camera box extends
	bool justGrounded;							// Whether the player object just became grounded

	// Variables for calculating default camera eye offset position
	float maxOffsetDistance;					// How far to the side/up the camera can look
	bool useOffset;								// Whether to use the offset
	float cameraSensitivity;					// Multiplier for the mouse

	// Cutscene variables
	CameraMovement* currentCameraMovement;		// The current camera movement. Only used for cutscenes.

	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

	/*************************************************************************************************/
	/*!
		\brief
			Updates the camera position given all offsets
	*/
	/*************************************************************************************************/
	void FixCameraPosition();

	/*************************************************************************************************/
	/*!
		\brief
			Update function to move the camera offsets in tune with player inputs
	*/
	/*************************************************************************************************/
	void UpdatePlayerCameraMovements();

	/*************************************************************************************************/
	/*!
		\brief
			Updates the camera box position

		\param dt
			The time elapsed since the previous frame
	*/
	/*************************************************************************************************/
	void UpdateCameraBox(double dt);
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_Camera_H_
