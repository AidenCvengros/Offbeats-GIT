/*************************************************************************************************/
/*!
\file AudioManager.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.2.25
\brief
    The audio manager class

    Functions include:
        + FILL

Copyright (c) 2025 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "AudioManager.h"
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
// Public Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Initializes the system.
*/
/*************************************************************************************************/
void AudioManager::Init()
{
	SDL_AudioSpec spec;							// The specs we want for our audio stream

	if (!SDL_Init(SDL_INIT_AUDIO))
	{
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return;
	}

	// Sets the audio stream specs
	spec.channels = 1;
	spec.format = SDL_AUDIO_F32;
	spec.freq = 8000;

	// Sets up the audio stream
	stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
	if (!stream)
	{
		SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
		return;
	}

	// Starts the audio stream
	SDL_ResumeAudioStreamDevice(stream);
}

/*************************************************************************************************/
/*!
	  \brief
		  Updates the system.

	  \param
		  The time elapsed since the previous frame.
*/
/*************************************************************************************************/
void AudioManager::Update(double dt)
{
	// Checks if the audio stream needs more data
	const int minAudio = (8000 * sizeof(float)) / 2; // a.k.a half empty
	if (SDL_GetAudioStreamQueued(stream) < minAudio)
	{
		float samples[512];						// The temp buffer of new data to input in the stream

		// Loops through the end of the stream adding new samples
		for (int i = 0; i < SDL_arraysize(samples); i++)
		{
			// Generates a 440Hz pure tone
			const int freq = 440;
			const float phase = currentSine * freq / 8000.0f;
			samples[i] = 0; // SDL_sinf(phase * 2 * SDL_PI_F);
			currentSine++;
		}

		// Wraps the sine sample value around to keep it in a normal number range
		currentSine %= 8000;

		// Feeds new data to the stream
		SDL_PutAudioStreamData(stream, samples, sizeof(samples));
	}
}

/*************************************************************************************************/
/*!
	  \brief
		  Draws the system to the screen.
*/
/*************************************************************************************************/
void AudioManager::Draw()
{

}

/*************************************************************************************************/
/*!
	  \brief
		  Shuts down the system.
*/
/*************************************************************************************************/
void AudioManager::Shutdown()
{

}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
