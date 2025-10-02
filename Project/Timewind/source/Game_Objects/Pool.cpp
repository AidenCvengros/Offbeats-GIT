/*************************************************************************************************/
/*!
\file Pool.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.7.14
\brief
    The pool game object for handling in-game resources

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Pool.h"
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
		Adds the given amount to the pool

	\param addAmount
		The amount to add to the pool

	\return
		How much was added to the pool (for instance will differ from addAmount if it hits max)
*/
/*************************************************************************************************/
float Pool::AddToPool(float addAmount)
{
	// Increases the amount in the pool
	currValue += addAmount;

	// Checks if the value overflowed
	if (overflow)
	{
		if (currValue > maxValue)
		{
			// Calculates how much was actually added to the pool
			float amountAdded = addAmount - (currValue - maxValue);

			// Caps the value
			currValue = maxValue;

			// Returns the amount added
			return amountAdded;
		}
		else if (currValue < 0)
		{
			// Calculates how much was actually added to the pool
			float amountAdded = addAmount - currValue;

			// Caps the value
			currValue = maxValue;

			// Returns the amount added
			return amountAdded;
		}
	}

	// Otherwise returns the amount added
	return addAmount;
}

/*************************************************************************************************/
/*!
	\brief
		Subtracts the given amount from the pool

	\param subtractAmount
		The amount to subtract from the pool

	\return
		How much was subtracted from the pool (for instance will differ from subtractAmount if it hits min)
*/
/*************************************************************************************************/
float Pool::SubtractFromPool(float subtractAmount)
{
	std::cout << "Enemy took " << subtractAmount << " damage." << std::endl;
	return AddToPool(-subtractAmount);
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
