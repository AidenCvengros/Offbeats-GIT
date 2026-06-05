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
	if (!SDL_Init(SDL_INIT_AUDIO))
	{
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return;
	}
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
	int minAudio = (8000 * sizeof(float)) / 2; // a.k.a half empty

	// Walks through the list of running audios
	for (auto i = audioList.begin(); i != audioList.end();)
	{
		// If we are running low on the audio
		int audioLeft = SDL_GetAudioStreamQueued(i->stream);
		if (audioLeft < minAudio)
		{
			// Checks if the sound should be repeated
			if (i->replays != 0)
			{
				// Feeds new data to the stream
				SDL_PutAudioStreamData(i->stream, i->audioData, i->audioLength);
				i->replays--;
			}
			// Checks if we are out of sound
			else if (audioLeft <= 0)
			{
				SDL_DestroyAudioStream(i->stream);
				i = audioList.erase(i);
				continue;
			}
		}

		i++;
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
	// Walks through the list of running audios
	for (auto i = audioList.begin(); i != audioList.end(); i++)
	{
		SDL_DestroyAudioStream(i->stream);
	}
	audioList.clear();
}

/*************************************************************************************************/
/*!
	\brief
		Plays an audio

	\param filename
		The name of the audio to play

	\param repeats
		The number of times to repeat it after. -1 loops indefinitely until ClearMusic() is called.
*/
/*************************************************************************************************/
void AudioManager::PlayAudio(std::string filename, int repeats)
{
	SDL_AudioSpec spec;							// The specs we want for our audio stream
	Audio newAudio;								// The new audio

	// Sets the replay value
	newAudio.replays = repeats;

	// Loads in the wav
	if (!SDL_LoadWAV(filename.c_str(), &spec, &newAudio.audioData, &newAudio.audioLength))
	{
		SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
		return;
	}

	// Sets up the audio stream
	newAudio.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
	if (!newAudio.stream)
	{
		SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
		return;
	}

	// Starts the audio stream
	SDL_ResumeAudioStreamDevice(newAudio.stream);

	audioList.push_back(newAudio);
}

/*************************************************************************************************/
/*!
	\brief
		Clears all looping audio
*/
/*************************************************************************************************/
void AudioManager::ClearMusic()
{
	// Walks through the list of running audios
	for (auto i = audioList.begin(); i != audioList.end();)
	{
		// Checks if it's a looping audio
		if (i->replays < 0)
		{
			SDL_DestroyAudioStream(i->stream);
			i = audioList.erase(i);
		}
		else
		{
			i++;
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
