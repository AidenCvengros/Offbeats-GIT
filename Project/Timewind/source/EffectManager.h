/*************************************************************************************************/
/*!
\file EffectManager.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.12.1
\brief
    Manages visual, particle, and audio effects

    Functions include:
        + EffectsManager
		+ ~EffectsManager
		+ StartEffect
		+ UpdateEffects

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_EffectManager_H_
#define Syncopatience_EffectManager_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"

#include <list>

#include "MapMatrix.h"
#include "GameObjectManager.h"
#include "TextureManager.h"

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
		Manages visual effects
*/
/*************************************************************************************************/
class EffectManager : public System
{
public:
	//---------------------------------------------------------------------------------------------
	// Public Consts
	//---------------------------------------------------------------------------------------------

	enum class EffectType
	{
		NullType = -1,
		Image,
		Particle,
		Sound,
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
			Constructor for the effect manager class

		\param gameObjectManager_
			The game object manager to store effect objects in

		\param textureManager_
			The texture object manager to store visual effect textures in
	*/
	/*************************************************************************************************/
	EffectManager();

	/*************************************************************************************************/
	/*!
		\brief
			Destructor for effect manager class
	*/
	/*************************************************************************************************/
	~EffectManager();

	/*************************************************************************************************/
	/*!
		\brief
			Initializes the system. Virtual function that must be overwritten by derived class
	*/
	/*************************************************************************************************/
	void Init();

	/*************************************************************************************************/
	/*!
		\brief
			Updates all currently active effects checking within the given map matrix

		\param mapMatrix
			The given map matrix

		\param dt
			How much time elapsed since the previous frame
	*/
	/*************************************************************************************************/
	void Update(double dt);

	/*************************************************************************************************/
	/*!
		\brief
			Draws the system to the screen. Virtual function that must be overwritten by derived class

		\param window
			The window being drawn to
	*/
	/*************************************************************************************************/
	void Draw(Window* window);

	/*************************************************************************************************/
	/*!
		\brief
			Shuts down the system. Virtual function that must be overwritten by derived class
	*/
	/*************************************************************************************************/
	void Shutdown();

	/*************************************************************************************************/
	/*!
		\brief
			Clears the entire list of active effects
	*/
	/*************************************************************************************************/
	void ClearEffectList();

	/*************************************************************************************************/
	/*!
		\brief
			Starts the given effect

		\param effectType
			The type of effect

		\param filename
			The file to play for the effect to start

		\param effectCoordinates
			The world coordinates of the effect

		\param effectRotation
			The rotation of the effect

		\param effectScale
			The scale of the effect

		\param effectFacingRight
			Boolean for whether the effect is going to the right (true) or the left (false)

		\param effectTime
			How long the effect lasts

		\param fadingStartTime
			How long in seconds the effect should go until it starts fading (0.0 will start fading immediately, if number is arbitrarily large the effect will never fade)
	*/
	/*************************************************************************************************/
	void StartEffect(EffectType effectType, std::string filename, glm::vec2 effectCoordinates, float effectRotation, glm::vec2 effectScale, bool effectFacingRight, double effectTime, glm::vec4 color, double fadingStartingTime);

	/*************************************************************************************************/
	/*!
		\brief
			Starts the given effect in a given map matrix tile

		\param effectType
			The type of effect

		\param filename
			The file to play for the effect to start

		\param effectTileCoordinates
			The tile coordinates of the effect

		\param effectFacingRight
			Boolean for whether the effect is going to the right (true) or the left (false)

		\param effectTime
			How long the effect lasts

		\param color
			The color of the effect

		\param fadingStartTime
			How long in seconds the effect should go until it starts fading (0.0 will start fading immediately, if number is arbitrarily large the effect will never fade)
	*/
	/*************************************************************************************************/
	void StartEffectInTile(EffectType effectType, std::string filename, std::pair<int, int> effectTileCoordinates, bool effectFacingRight, double effectTime, glm::vec4 color, double fadingStartingTime);
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Structures
	//---------------------------------------------------------------------------------------------

	typedef struct
	{
		bool effectActive;						// Denotes whether the effect is active
		EffectType effectType;					// The type of effect
		GameObject* object;						// The game object for the effect
		double effectTotalTime;					// The total length of the effect
		double effectTimer;						// How much time is left in the current effect
		double fadingStartingTime;				// How long into the effect it should start fading
		glm::vec4 startingColor;				// The initial color of the effect
		glm::vec4 fadeColor;					// The color that the effect fades to
	}EffectStruct;
	
	//---------------------------------------------------------------------------------------------
	// Private Variables
	//---------------------------------------------------------------------------------------------

	std::list<EffectStruct> activeEffects;		// Holds the active effects
	
	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

	/*************************************************************************************************/
	/*!
		\brief
			Clears the given effect

		\param effect
			The effect to clear
	*/
	/*************************************************************************************************/
	void ClearEffect(EffectStruct& effect);
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_EffectManager_H_
