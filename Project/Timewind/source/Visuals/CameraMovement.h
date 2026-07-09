/*************************************************************************************************/
/*!
\file CameraMovement.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.6.23
\brief
    Class to hold the information for a 3d movement

    Functions include:
        + [FILL]

Copyright (c) 2025 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_CameraMovement_H_
#define Syncopatience_CameraMovement_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base include
#include "../Engine/stdafx.h"

//-------------------------------------------------------------------------------------------------
// Forward References
//-------------------------------------------------------------------------------------------------

class Menu;

//-------------------------------------------------------------------------------------------------
// Public Constants
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Classes
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		The class for 3d movement tracking
*/
/*************************************************************************************************/
class CameraMovement
{
public:
	//---------------------------------------------------------------------------------------------
	// Public Consts
	//---------------------------------------------------------------------------------------------

	enum class MovementType
	{
		Translation,
		Revolution,
		Zoom,
		Max
	};
	
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
			Constructor for the menu class

		\param _optionVisual
			The visual representation of the menu option

		\param _submenu
			The submenu this option is interacting with

		\param _interaction
			What action this button takes on the submenu
	*/
	/*************************************************************************************************/
	CameraMovement(MovementType _movementType, double _length, glm::vec3 _startingPosition, glm::vec3 _lookAtPosition, glm::vec3 _magnitude, float _startingPerspective, float _endingPerspective) : movementType(_movementType), length(_length), 
		startingPos(_startingPosition), lookAtPos(_lookAtPosition), magnitude(_magnitude),
		startingPerspective(_startingPerspective), endingPerspective(_endingPerspective),
		timeRemaining(_length), totalLookAtOffset({ 0.0f }), totalTranslationOffset({ 0.0f }) {}

	/*************************************************************************************************/
	/*!
		 \brief
			Destructor for menu class
	*/
	/*************************************************************************************************/
	~CameraMovement() {}

	/*************************************************************************************************/
	/*!
		\brief
			Updates the movement

		\param dt
			The time elapsed since the previous frame
	*/
	/*************************************************************************************************/
	virtual void Update(double dt);

	/*************************************************************************************************/
	/*!
		\brief
			Returns the movement type

		\return
			The movement type
	*/
	/*************************************************************************************************/
	MovementType GetMovementType() { return movementType; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the amount of time in seconds of the movement

		\return
			The remaining time
	*/
	/*************************************************************************************************/
	double GetRemainingTime() { return timeRemaining; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the magnitude of the movement

		\return
			Gets the 3d magnitude vector
	*/
	/*************************************************************************************************/
	glm::vec3 GetMagnitude() { return magnitude; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns how far the look at position has moved

		\return
			The look at offset
	*/
	/*************************************************************************************************/
	glm::vec3 GetLookAtOffset() { return totalLookAtOffset; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the translation the movement has produced

		\return
			The translation offset
	*/
	/*************************************************************************************************/
	glm::vec3 GetTranslationOffset() { return totalTranslationOffset; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the rotation the movement has produced

		\return
			Gets the rotation offset
	*/
	/*************************************************************************************************/
	glm::vec3 GetPosition() { return startingPos + totalTranslationOffset; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the current perspective ratio

		\return
			Gets the perspective ratio
	*/
	/*************************************************************************************************/
	float GetPerspective();

private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------

	MovementType movementType;					// The movement type
	double length;								// The length of the movement in seconds
	glm::vec3 magnitude;						// The magnitude of the movement (how it's used varies by movement type)
	glm::vec3 startingPos;						// Where the camera starts
	glm::vec3 lookAtPos;						// The point the camera is looking at
	float startingPerspective;					// The perspective ratio the camera starts at
	float endingPerspective;					// The perspective ratio the camera ends at

	double timeRemaining;						// The amount of time remaining
	glm::vec3 totalLookAtOffset;				// The total offset of where the camera should be looking
	glm::vec3 totalTranslationOffset;			// The total offset the movement has produced so far
	
	//---------------------------------------------------------------------------------------------
	// Private Structures
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Variables
	//---------------------------------------------------------------------------------------------
	
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

#endif // Syncopatience_CameraMovement_H_
