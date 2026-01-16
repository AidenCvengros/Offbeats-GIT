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
			Returns the 3-dimensional position of the centered object

		\return
			The position of the centered object
	*/
	/*************************************************************************************************/
	//glm::vec4 GetCenteredObject3DPosition();

	/*************************************************************************************************/
	/*!
		\brief
			Sets a game object for the camera to center on

		\param object
			The new centered object
	*/
	/*************************************************************************************************/
	void SetCenteredObject(Player* object);
	
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
	
	//bool rotationChanged;						// Boolean for updating camera rotation				THIS VARIABLE NEEDS TO TRACK WHETHER THE TARGET GO HAS CHANGED BEFORE IT WORKS
	bool perspectiveChanged;					// Boolean for updating camera perspective matrix

	bool inCutscene;							// Cutscenes hijack the normal gameplay camera

	glm::vec3 upVector;							// The up vector for the camera
	Player* centeredObject;						// The game object that the camera is focusing on (probably the player)
	glm::vec2 cameraBoxPos;						// The position of the camera box

	glm::mat4 viewMat;							// The view matrix for the camera
	glm::mat4 perspMat;							// The perspective matrix for the camera

	float aspectRatio;							// The aspect ratio of the camera view
	float fov;									// The field of view of the camera view

	double relativePosX;						// Tracks horizontal mouse movement for camera movement
	double relativePosY;						// Tracks vertical mouse movement for camera movement

	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

	/*************************************************************************************************/
	/*!
		\brief
			Updates the relative camera position
	*/
	/*************************************************************************************************/
	void UpdateRelativePosition();
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_Camera_H_
