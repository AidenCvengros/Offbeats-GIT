/*************************************************************************************************/
/*!
\file Texture.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2024.3.11
\brief
    The texture class holds textures for rendering detailed visuals

    Functions include:
        + [FILL]

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_Texture_H_
#define Syncopatience_Texture_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"

// has the string class for filenames
#include <string>

// Includes glfw library for texture management
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

// Gets the window data to write buffers to
class Window;

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
		The texture class. It's a wrapper for holding textures.
*/
/*************************************************************************************************/
class Texture
{
public:
	//---------------------------------------------------------------------------------------------
	// Public Consts
	//---------------------------------------------------------------------------------------------
	
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
			Constructor for the camera class

		param window
			The pointer to the game window

		\param filename_
			The name and location of the texture file
	*/
	/*************************************************************************************************/
	Texture(Window* window, std::string filename_);

	/*************************************************************************************************/
	/*!
		\brief
			Destructor for the camera class
	*/
	/*************************************************************************************************/
	~Texture();

	/*************************************************************************************************/
	/*!
		\brief
			Frees the texture from memory

		param window
			The pointer to the game window
	*/
	/*************************************************************************************************/
	void Free(Window* window);

	/*************************************************************************************************/
	/*!
		\brief
			Returns whether the game object should be rendered

		\return
			Whether the game object should be rendered
	*/
	/*************************************************************************************************/
	VkDescriptorSet* GetDescriptorSet() { return &descriptorSet; }

	/*************************************************************************************************/
	/*!
		\brief
			Determines whether the given filename matches this texture's filename

		\return
			Whether the two filenames are the same
	*/
	/*************************************************************************************************/
	friend bool operator==(const Texture& lhs, const std::string& rhs);
	friend bool operator==(const std::string& lhs, const Texture& rhs);
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Structures
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Variables
	//---------------------------------------------------------------------------------------------
	
	std::string filename;						// The name of the texture file

	VkBuffer textureBuffer;						// The buffer that holds loaded textures
	VkDeviceMemory textureBufferMemory;			// The memory pointer for the texture buffer
	VkImage textureImage;						// The image texture
	VkDeviceMemory textureImageMemory;			// The memory pointer to the image texture

	VkImageView textureImageView;				// The image view object for textures
	VkSampler textureSampler;					// The texture image sampler
	
	VkDescriptorSet descriptorSet;				// The base descriptor set memory

	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

	/*********************************************************************************************/
	/*!
		\brief
			Creates an image object with the given parameters

		\param window
			The pointer to the game window

		\param width
			The width of the image

		\param height
			The height of the image

		\param format
			Format of the image

		\param tiling
			Whether and how the image should be tiled

		\param usage
			Tells how the image will be used

		\param properties
			Defines the properties of the image for Vulkan

		\param image
			Output: The image texture

		\param imageMemory
			Output: The image pointer in memory
	*/
	/*********************************************************************************************/
	void CreateImage(
		Window* window,
		uint32_t width, uint32_t height,
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkImage& image,
		VkDeviceMemory& imageMemory);

	/*********************************************************************************************/
	/*!
		\brief
			Makes sure image data is formatted correctly to be written to the screen

		\param window
			The pointer to the game window
	
		\param image
			The image being formatted
	
		\param format
			The format for the image
	
		\param oldLayout
			The old data layout
	
		\param newLayout
			The new data layout
	*/
	/*********************************************************************************************/
	void TransitionImageLayout(Window* window, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	/*********************************************************************************************/
	/*!
		\brief
			Converts the given buffer data into a Vulkan image

		\param window
			The pointer to the game window
	
		\param buffer
			The given buffer data
	
		\param image
			The vulkan image converted to
	
		\param width
			The width the image
	
		\param height
			The height of the image
	*/
	/*********************************************************************************************/
	void CopyBufferToImage(Window* window, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	/*********************************************************************************************/
	/*!
		\brief
			Creates a texture sampler object

		\param window
			The pointer to the game window
	*/
	/*********************************************************************************************/
	void CreateTextureSampler(Window* window);

	/*********************************************************************************************/
	/*!
		\brief
			Creates a descriptor set for the texture

		\param window
			The pointer to the game window
	*/
	/*********************************************************************************************/
	void CreateTextureDescriptorSet(Window* window);
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_Texture_H_
