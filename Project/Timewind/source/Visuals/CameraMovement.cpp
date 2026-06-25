/*************************************************************************************************/
/*!
\file SubmenuOption.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.4.29
\brief
    The class for the menu option that interacts with submenus

    Functions include:
        + [FILL]

Copyright (c) 2025 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "CameraMovement.h"
#include "../Engine/cppShortcuts.h"

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
		Updates the movement

	\param dt
		The time elapsed since the previous frame
*/
/*************************************************************************************************/
void CameraMovement::Update(double dt)
{
	// Checks that there's still time in the simulation
	if (timeRemaining > 0.0)
	{
		// If time remaining is less than dt, updates dt for the purposes of these calcs
		if (timeRemaining < dt)
		{
			dt = timeRemaining;
		}

		// Decrements the time remaining
		timeRemaining = std::max(0.0, timeRemaining - dt);

		// Checks movement type
		if (movementType == MovementType::Translation)
		{
			totalTranslationOffset += (float)(dt / length) * magnitude;
		}
		else if (movementType == MovementType::Revolution)
		{
			// Gets the relative vector
			glm::vec3 relativeVec = GetPosition() - lookAtPos;

			// Calculates current angle
			float horizAngle = atan(relativeVec.z / relativeVec.x);
			//float vertAngle = acos(relativeVec.y / relativeVec)

			// Calculates the new positions based on how much time has elapsed since the previous frame
			double timeElapsedPercentage = dt / length;
			totalTranslationOffset.x += cosf(horizAngle + timeElapsedPercentage * magnitude.x) * magnitude.z;
			//totalTranslationOffset.y = sinf(timeElapsedPercentage * magnitude.y) * magnitude.z;
			totalTranslationOffset.z += sinf(horizAngle + timeElapsedPercentage * magnitude.x) * magnitude.z;
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
