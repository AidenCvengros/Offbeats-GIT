/*************************************************************************************************/
/*!
\file TextureManager.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.5.7
\brief
    Manages textures and ensures that multiple textures of the same file don't get created

    Functions include:
        + System::System
		+ Systme::~System

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "TextureManager.h"
#include "cppShortcuts.h"

// Includes the texture class to be managed
#include "Texture.h"
#include "Font.h"

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
		Constructor for the texture manager class
*/
/*************************************************************************************************/
TextureManager::TextureManager() : System(SystemTypes::textureManager), textureList(), defaultFont(NULL)
{
	
}

/*************************************************************************************************/
/*!
	\brief
		Destructor for the texture manager class
*/
/*************************************************************************************************/
TextureManager::~TextureManager()
{

}

/*************************************************************************************************/
/*!
	\brief
		Initializes the texture manager
*/
/*************************************************************************************************/
void TextureManager::Init()
{
	
}

/*************************************************************************************************/
/*!
	\brief
		Empties the texture list
*/
/*************************************************************************************************/
void TextureManager::Clear()
{
	// Walks through the texture list
	for (std::list<Texture*>::iterator it = textureList.begin(); it != textureList.end();)
	{
		// Frees the texture
		delete* it;
		it++;
	}

	// Walks through the font list
	for (auto it = fontList.begin(); it != fontList.end();)
	{
		// Frees the font
		delete* it;
		it++;
	}

	// Clears the texture list
	textureList.clear();
	fontList.clear();
	defaultFont = NULL;
}

/*************************************************************************************************/
/*!
	\brief
		Shuts down the texture manager and all the textures in it.
*/
/*************************************************************************************************/
void TextureManager::Shutdown()
{
	Clear();
}

/*************************************************************************************************/
/*!
	\brief
		Adds a texture to the manager's list.

	\param filename_
		The name of the texture file

	\return
		Pointer to the new texture
*/
/*************************************************************************************************/
Texture* TextureManager::AddTexture(std::string filename_)
{
	// Loops through the texture list to see if this file has already been loaded
	//auto it = std::find(textureList.begin(), textureList.end(), filename_);
	auto it = textureList.begin();
	for (; it != textureList.end(); it++)
	{
		if (**it == filename_)
		{
			break;
		}
	}
	
	// If this file has been loaded
	if (it != textureList.end())
	//if (textureList.size() && *(textureList.front()) == filename_)
	{
		// Return that texture
		return *it;
		//return textureList.front();
	}
	// If the file has not been loaded yet
	else
	{
		// Creates the new texture
		Texture* newTexture = new Texture(filename_);

		// Puts the new texture on the list
		textureList.push_back(newTexture);

		// Returns the new texture
		return newTexture;
	}
}

/*************************************************************************************************/
/*!
	\brief
		Adds a font to the manager's list.

	\param filename_
		The name of the font file

	\return
		Pointer to the new font
*/
/*************************************************************************************************/
Font* TextureManager::AddFont(std::string filename_)
{
	// Loops through the font list to see if this file has already been loaded
	auto it = fontList.begin();
	for (; it != fontList.end(); it++)
	{
		if ((*it)->GetFilename() == filename_)
		{
			break;
		}
	}

	// If this file has been loaded
	if (it != fontList.end())
	{
		// Return that font
		return *it;
	}
	// If the file has not been loaded yet
	else
	{
		// Creates the new texture
		Font* newFont = new Font(filename_);

		// Puts the new texture on the list
		fontList.push_back(newFont);

		// Returns the new texture
		return newFont;
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
