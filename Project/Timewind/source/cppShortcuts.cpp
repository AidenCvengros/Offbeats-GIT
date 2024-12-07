/*************************************************************************************************/
/*!
\file cppShortcuts.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.12.5
\brief
    Include file for .cpp files that simplifies function calls, especially engine calls

    Functions include:
        + ConvertMapCoordToWorldCoord
		+ ConvertMapCoordsToWorldCoords
		+ ConvertWorldCoordToMapCoord
		+ ConvertWorldCoordsToMapCoords

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "cppShortcuts.h"

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
		Converts a given map matrix tile coordinate to a world coordinate

	\param mapCoordinate
		The given map tile coordinate

	\return
		The corresponding location in world coordinates
*/
/*************************************************************************************************/
double ConvertMapCoordToWorldCoord(int mapCoordinate)
{
	return (double)mapCoordinate * 2.0;
}

/*************************************************************************************************/
/*!
	\brief
		Converts the given map matrix tile coordinates to world coordinates

	\param mapXCoordinate
		The given map tile x coordinate

	\param mapYCoordinate
		The given map tile y coordinate

	\return
		The corresponding location in world coordinates
*/
/*************************************************************************************************/
std::pair<double, double> ConvertMapCoordsToWorldCoords(int mapXCoordinate, int mapYCoordinate)
{
	return std::make_pair(ConvertMapCoordToWorldCoord(mapXCoordinate), ConvertMapCoordToWorldCoord(mapYCoordinate));
}

/*************************************************************************************************/
/*!
	\brief
		Converts a given world matrix tile coordinate to a map coordinate

	\param worldCoordinate
		The given world tile coordinate

	\return
		The corresponding location in map coordinates
*/
/*************************************************************************************************/
int ConvertWorldCoordToMapCoord(double worldCoordinate)
{
	return std::floor(worldCoordinate) / 2;
}

/*************************************************************************************************/
/*!
	\brief
		Converts the given world matrix tile coordinates to map coordinates

	\param worldXCoordinate
		The given world tile x coordinate

	\param worldYCoordinate
		The given world tile y coordinate

	\return
		The corresponding location in map coordinates
*/
/*************************************************************************************************/
std::pair<int, int> ConvertWorldCoordsToMapCoords(double worldXCoordinate, double worldYCoordinate)
{
	return std::make_pair(ConvertWorldCoordToMapCoord(worldXCoordinate), ConvertWorldCoordToMapCoord(worldYCoordinate));
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
