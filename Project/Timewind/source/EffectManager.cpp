/*************************************************************************************************/
/*!
\file EffectManager.cpp
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

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "EffectManager.h"

#include "cppShortcuts.h"

// Includes the manager for textures, audio, and particles
#include "TextureManager.h"

#include "Engine.h"
#include "SceneManager.h"

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
		Constructor for the effect manager class

	\param gameObjectManager_
		The game object manager to store effect objects in
*/
/*************************************************************************************************/
EffectManager::EffectManager() : activeEffects(16)
{
	
}

/*************************************************************************************************/
/*!
	\brief
		Destructor for effect manager class
*/
/*************************************************************************************************/
EffectManager::~EffectManager()
{

}

/*************************************************************************************************/
/*!
	\brief
		Initializes the system. Virtual function that must be overwritten by derived class
*/
/*************************************************************************************************/
void EffectManager::Init()
{

}

/*************************************************************************************************/
/*!
	\brief
		Updates all currently active effects checking within the given map matrix

	\param dt
		How much time elapsed since the previous frame
*/
/*************************************************************************************************/
void EffectManager::Update(double dt)
{
	for (std::list<EffectStruct>::iterator i = activeEffects.begin(); i != activeEffects.end();)
	{
		// Checks that the effect is active
		if (i->effectActive)
		{
			// Decrements the timer
			i->effectTimer -= dt;

			// Checks if the effect is over
			if (i->effectTimer <= 0.0)
			{
				// If the effect is over, deletes it
				ClearEffect(*i);
				i = activeEffects.erase(i);
			}
			// Otherwise moves on to the next effect in the list
			else
			{
				i++;
			}
		}
		else
		{
			i++;
		}
	}
}

/*************************************************************************************************/
/*!
	\brief
		Draws the system to the screen. Virtual function that must be overwritten by derived class

	\param window
		The window being drawn to
*/
/*************************************************************************************************/
void EffectManager::Draw(Window* window)
{

}

/*************************************************************************************************/
/*!
	\brief
		Shuts down the system. Virtual function that must be overwritten by derived class
*/
/*************************************************************************************************/
void EffectManager::Shutdown()
{

}

/*************************************************************************************************/
/*!
	\brief
		Clears the entire list of active effects
*/
/*************************************************************************************************/
void EffectManager::ClearEffectList()
{
	activeEffects.clear();
}

/*************************************************************************************************/
/*!
	\brief
		Starts the given effect

	\param effectType
		The type of effect

	\param filename
		The file to play for the effect to start

	\param effectXCoordinate
		The x world coordinate of the effect

	\param effectYCoordinate
		The y world coordinate of the effect

	\param effectFacingRight
		Boolean for whether the effect is going to the right (true) or the left (false)

	\param effectTime
		How long the effect lasts
*/
/*************************************************************************************************/
void EffectManager::StartEffect(EffectManager::EffectType effectType, std::string filename, double effectXCoordinate, double effectYCoordinate, bool effectFacingRight, double effectTime)
{
	// Starts a visual effect
	if (effectType == EffectType::Image)
	{
		// Makes the texture
		Texture* newTexture = _TextureManager->AddTexture(filename);

		// Adjusts the effect for facing left or right


		// makes the effect game object
		GameObject* effectObject = new GameObject(glm::vec2(effectXCoordinate, effectYCoordinate), 0.0f, glm::vec2(1.0f, 1.0f), 60, newTexture, false, {0.6f, 0.2f, 0.1f, 0.6f});
		_GameObjectManager->AddGameObject(effectObject);

		// Stores the new effect in the active effects list
		EffectStruct newEffect = { true, effectType, effectObject, effectTime };
		activeEffects.push_back(newEffect);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Starts the given effect centered at a given map matrix tile

	\param effectType
		The type of effect

	\param filename
		The file to play for the effect to start

	\param effectXTileCoordinate
		The x tile coordinate of the effect

	\param effectYTileCoordinate
		The y tile coordinate of the effect

	\param effectFacingRight
		Boolean for whether the effect is going to the right (true) or the left (false)

	\param effectTime
		How long the effect lasts
*/
/*************************************************************************************************/
void EffectManager::StartEffectAtTile(EffectManager::EffectType effectType, std::string filename, int effectXTileCoordinate, int effectYTileCoordinate, bool effectFacingRight, double effectTime)
{
	StartEffect(effectType, filename, ConvertMapCoordToWorldCoord(effectXTileCoordinate), ConvertMapCoordToWorldCoord(effectYTileCoordinate), effectFacingRight, effectTime);
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Clears the given effect

	\param effect
		The effect to clear
*/
/*************************************************************************************************/
void EffectManager::ClearEffect(EffectStruct& effect)
{
	// Stops the effect
	effect.effectActive = false;

	// Clears visual effects
	if (effect.effectType == EffectType::Image)
	{
		// Deletes the effect object
		effect.object->SetToBeDestroyed(true);
	}
}
