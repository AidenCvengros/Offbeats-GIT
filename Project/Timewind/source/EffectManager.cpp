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
			// Otherwise updates the effect
			else
			{
				// Updates the fade if applicable
				if (i->effectTotalTime - i->effectTimer > i->fadingStartingTime)
				{
					// Calculates interpolation between starting color and fade color
					float currentFadeTime = (i->effectTotalTime - i->effectTimer) - i->fadingStartingTime;
					float totalFadeTime = i->effectTotalTime - i->fadingStartingTime;
					i->object->SetColor(i->startingColor + ((currentFadeTime / totalFadeTime) * (i->fadeColor - i->startingColor)));
				}
				
				// Moves on to the next effect in the list
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

	\param color
		The color of the effect

	\param fadingStartTime
		How long in seconds the effect should go until it starts fading (0.0 will start fading immediately, if number is arbitrarily large the effect will never fade)
*/
/*************************************************************************************************/
void EffectManager::StartEffect(EffectManager::EffectType effectType, std::string filename, glm::vec2 effectCoordinates, float effectRotation, glm::vec2 effectScale, bool effectFacingRight, double effectTime, glm::vec4 color, double fadingStartTime)
{
	// Starts a visual effect
	if (effectType == EffectType::Image)
	{
		// Makes the texture
		Texture* newTexture = _TextureManager->AddTexture(filename);

		// makes the effect game object
		GameObject* effectObject = new GameObject(effectCoordinates, effectRotation, effectScale, 60, effectFacingRight, newTexture, false, color);
		_GameObjectManager->AddGameObject(effectObject);

		// Stores the new effect in the active effects list
		EffectStruct newEffect = { true, effectType, effectObject, effectTime, effectTime, fadingStartTime, color, { glm::vec3(color), 0.0f } };
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
void EffectManager::StartEffectInTile(EffectManager::EffectType effectType, std::string filename, std::pair<int, int> effectTileCoordinates, bool effectFacingRight, double effectTime, glm::vec4 color, double fadingStartTime)
{
	StartEffect(effectType, filename, ConvertMapCoordsToWorldCoords(effectTileCoordinates), 0.0f, { 2.0f, 2.0f }, effectFacingRight, effectTime, color, fadingStartTime);
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
