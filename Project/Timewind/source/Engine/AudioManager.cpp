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

// Additional includes
#include <SDL3_mixer/SDL_mixer.h>

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
	//if (!SDL_Init(SDL_INIT_AUDIO))
	//{
	//	SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
	//	return;
	//}

	if (!MIX_Init())
	{
		SDL_Log("Couldn't initialize SDL_Mix: %s", SDL_GetError());
		return;
	}

	// Create the mixer
	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (!mixer)
	{
		SDL_Log("Couldn't create mixer on default device: %s", SDL_GetError());
		return;
	}

	// Creates the mixer tracks
	trackList.push_back({ true, MIX_CreateTrack(mixer) });
	MIX_TagTrack(trackList[0].second, "Background");
	for (int i = 1; i < 32; i++)
	{
		trackList.push_back({ false, MIX_CreateTrack(mixer) });
		MIX_TagTrack(trackList[i].second, "Effect");
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
	// Clears the sdl mixer
	MIX_Quit();
}

/*************************************************************************************************/
/*!
	\brief
		Plays an audio

	\param filename
		The name of the audio to play

	\param repeats
		The number of times to repeat it after. -1 loops indefinitely until ClearMusic() is called.

	\param channelID
		The channel to play this audio on. If value is negative, will reserve you a channel

	\param reserve
		Whether the channel should free itself after the audio finishes playing
*/
/*************************************************************************************************/
void AudioManager::PlayAudio(std::string filename, int repeats, int& channelID, bool reserve)
{
	SDL_AudioSpec spec;							// The specs we want for our audio stream

	// Loads in the audio file
	MIX_Audio* audio = MIX_LoadAudio(mixer, filename.c_str(), false);
	if (!audio)
	{
		SDL_Log("Couldn't load %s: %s", filename, SDL_GetError());
	}

	// Plays the track
	SDL_PropertiesID options = SDL_CreateProperties();
	SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, repeats);
	if (channelID < 0 || channelID >= 32)
	{
		channelID = FindOpenTrack();
	}
	if (channelID != 0)
	{
		trackList[channelID].first = reserve;
	}
	else
	{
		trackList[0].first = true;
	}
	MIX_SetTrackAudio(trackList[channelID].second, audio);
	MIX_PlayTrack(trackList[channelID].second, options);
	SDL_DestroyProperties(options);

	//// Loads in the wav
	//if (!SDL_LoadWAV(filename.c_str(), &spec, &newAudio.audioData, &newAudio.audioLength))
	//{
	//	SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
	//	return;
	//}
	//
	//// Sets up the audio stream
	//newAudio.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
	//if (!newAudio.stream)
	//{
	//	SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
	//	return;
	//}
	//
	//// Starts the audio stream
	//SDL_ResumeAudioStreamDevice(newAudio.stream);
	//newAudio.format = spec.format;
	//audioList.push_back(newAudio);
}

/*************************************************************************************************/
/*!
	\brief
		Clears all looping audio
*/
/*************************************************************************************************/
void AudioManager::ClearMusic()
{
	// Stops all tracks
	MIX_StopAllTracks(mixer, 0);
}

/*************************************************************************************************/
/*!
	\brief
		Pauses all audio
*/
/*************************************************************************************************/
void AudioManager::PauseAudio()
{
	MIX_PauseTag(mixer, "Effect");
	MIX_SetTagGain(mixer, "Background", 0.5f);
}

/*************************************************************************************************/
/*!
	\brief
		Restarts all paused audio
*/
/*************************************************************************************************/
void AudioManager::RestartAudio()
{
	// Walks through the list of running audios
	MIX_PlayTag(mixer, "Effect", 0);
	MIX_SetTagGain(mixer, "Background", 1.0f);
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Returns an open audio track. Will select one randomly if none are free

	\return
		The index for an open audio track
*/
/*************************************************************************************************/
int AudioManager::FindOpenTrack()
{
	for (int i = 1; i < trackList.size(); i++)
	{
		if (trackList[i].first == false)
		{
			return i;
		}
	}

	// Otherwise choose a random track
	return (rand() % 31) + 1;
}
