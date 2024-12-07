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

#include "stdafx.h"

// Base game object class
#include "GameObject.h"

// glm matrix and math functionality
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Player class for the camera to be centered on
#include "Player.h"

//-------------------------------------------------------------------------------------------------
// Forward References
//-------------------------------------------------------------------------------------------------

class Scene;

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
	Camera(glm::vec2 pos, float rot, glm::vec2 sca, Player* centeredObject_, float aspectRatio_, float fieldOfView) :
		GameObject(pos, rot, sca, 0, false),
		//rotationChanged(true),
		perspectiveChanged(true),
		upVector(glm::vec3(0.0f, -1.0f, 0.0f)),
		centeredObject(centeredObject_),
		viewMat(glm::mat4(0.0f)),
		perspMat(glm::mat4(0.0f)),
		aspectRatio(aspectRatio_),
		fov(fieldOfView) {}
	
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
	
		\param inputManager
			The input manager
	*/
	/*************************************************************************************************/
	void Update(double dt, InputManager* inputManager);

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

	glm::vec3 upVector;							// The up vector for the camera
	Player* centeredObject;						// The game object that the camera is focusing on (probably the player)

	glm::mat4 viewMat;							// The view matrix for the camera
	glm::mat4 perspMat;							// The perspective matrix for the camera

	float aspectRatio;							// The aspect ratio of the camera view
	float fov;									// The field of view of the camera view

	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_Camera_H_
