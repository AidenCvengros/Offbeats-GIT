/*************************************************************************************************/
/*!
\file Texture.cpp
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

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "Texture.h"

#include "cppShortcuts.h"

// Texture headers
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Window.h"

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
		Constructor for the camera class

	\param window
		The pointer to the game window

	\param filename_
		The name and location of the texture file
*/
/*************************************************************************************************/
Texture::Texture(Window* window, std::string filename_)
{
	// Sets the given variables
	filename = filename_;

	// Loads in the texture
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = (uint64_t)texWidth * texHeight * 4;

	// Checks that an image was loaded in this way
	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image!");
	}

	// Creates a buffer in memory for the texture buffer
	window->CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, textureBuffer, textureBufferMemory);

	// Copies the image data over to the buffer
	void* data;
	vkMapMemory(window->GetLogicalDevice(), textureBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(window->GetLogicalDevice(), textureBufferMemory);

	// Frees up stb image
	stbi_image_free(pixels);

	// Creates the image object
	CreateImage(window, texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	// Transitions the texture buffer data to the image
	TransitionImageLayout(window, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(window, textureBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	TransitionImageLayout(window, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	// Cleans up the texture buffer
	vkDestroyBuffer(window->GetLogicalDevice(), textureBuffer, NULL);
	vkFreeMemory(window->GetLogicalDevice(), textureBufferMemory, NULL);

	// Constructs the image view object
	textureImageView = window->CreateImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB);

	// Creates the texture sampler
	CreateTextureSampler(window);

	// Creates descriptor sets for the texture
	CreateTextureDescriptorSet(window);
}

/*************************************************************************************************/
/*!
		\brief
			Constructor for the texture class

		\param window
			The pointer to the game window

		\param textureWidth
			The width of the new texture

		\param textureHeight
			The height of the new texture

		\param imageFormat
			The format for the new texture
*/
/*************************************************************************************************/
Texture::Texture(Window* window, int textureWidth, int textureHeight, VkFormat imageFormat)
{
	// Creates a buffer in memory for the texture buffer
	window->CreateBuffer(textureWidth * textureHeight * 4, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, textureBuffer, textureBufferMemory);

	// Creates the image object
	CreateImage(window, textureWidth, textureHeight, imageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	// Transitions the texture buffer data to the image
	TransitionImageLayout(window, textureImage, imageFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(window, textureBuffer, textureImage, static_cast<uint32_t>(textureWidth), static_cast<uint32_t>(textureHeight));
	TransitionImageLayout(window, textureImage, imageFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	// Cleans up the texture buffer
	vkDestroyBuffer(window->GetLogicalDevice(), textureBuffer, NULL);
	vkFreeMemory(window->GetLogicalDevice(), textureBufferMemory, NULL);

	// Constructs the image view object
	textureImageView = window->CreateImageView(textureImage, imageFormat);

	// Creates the texture sampler
	CreateTextureSampler(window);

	// Creates descriptor sets for the texture
	CreateTextureDescriptorSet(window);

	// Now that the texture has been created, turns off the freed flag
	freed = false;
}

/*************************************************************************************************/
/*!
	\brief
		Destructor for the camera class
*/
/*************************************************************************************************/
Texture::~Texture()
{
	Free();
}

/*************************************************************************************************/
/*!
	\brief
		Frees the texture from memory
*/
/*************************************************************************************************/
void Texture::Free()
{
	// Checks that the texture hasn't already been freed
	if (freed == false)
	{
		// Ends the render pass
		_Window->WaitForDrawFinished();

		// Cleans up the descriptor sets
		vkFreeDescriptorSets(_Window->GetLogicalDevice(), _Window->GetDescriptorPool(), 1, &descriptorSet);

		// Cleans up the texture image view objects
		vkDestroySampler(_Window->GetLogicalDevice(), textureSampler, NULL);
		vkDestroyImageView(_Window->GetLogicalDevice(), textureImageView, NULL);

		// Cleans up the texture objects
		vkDestroyImage(_Window->GetLogicalDevice(), textureImage, NULL);
		vkFreeMemory(_Window->GetLogicalDevice(), textureImageMemory, NULL);

		// Sets the freed flag
		freed = true;
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

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
void Texture::CreateImage(
	Window* window,
	uint32_t width, uint32_t height,
	VkFormat format,
	VkImageTiling tiling,
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkImage& image,
	VkDeviceMemory& imageMemory)
{
	// Creates the texture image object info struct
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = static_cast<uint32_t>(width);
	imageInfo.extent.height = static_cast<uint32_t>(height);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

	// Creates the texture image object
	if (vkCreateImage(window->GetLogicalDevice(), &imageInfo, NULL, &textureImage) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create image!");
	}

	// Ensures that the device can support the memory requirements for the texture
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(window->GetLogicalDevice(), image, &memRequirements);

	// Allocates the memory for the texture image
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = window->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// Allocates the memory
	if (vkAllocateMemory(window->GetLogicalDevice(), &allocInfo, NULL, &imageMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate image memory!");
	}

	// Binds the texture image into memory
	vkBindImageMemory(window->GetLogicalDevice(), image, imageMemory, 0);
}

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
void Texture::TransitionImageLayout(Window* window, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	// Starts the command buffer
	VkCommandBuffer commandBuffer = window->BeginSingleTimeCommands();

	// Defines the data barrier to be overcome
	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	// Sets the start and end points
	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	// Determines undefined transition
	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		// Sets the transition properties for undefined
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	// Determines destination transition
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		// Sets the transition properties for destination transitions
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	// Otherwise we can't handle the image
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	// Trans Rights
	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, NULL,
		0, NULL,
		1, &barrier);

	// Closes the command buffer
	window->EndSingleTimeCommands(commandBuffer);
}

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
void Texture::CopyBufferToImage(Window* window, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	// Opens a command buffer
	VkCommandBuffer commandBuffer_ = window->BeginSingleTimeCommands();

	// Defines what part of the buffer to copy
	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	// Copies the buffer
	vkCmdCopyBufferToImage(commandBuffer_, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	// Closes the command buffer
	window->EndSingleTimeCommands(commandBuffer_);
}

/*********************************************************************************************/
/*!
	\brief
		Creates a texture sampler object

	\param window
		The pointer to the game window
*/
/*********************************************************************************************/
void Texture::CreateTextureSampler(Window* window)
{
	// Gets the properties of the graphics device
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(window->GetPhysicalDevice(), &properties);

	// Sets the properties of the texture sampler
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	// Checks if the texture sampler gets created correctly
	if (vkCreateSampler(window->GetLogicalDevice(), &samplerInfo, NULL, &textureSampler) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}

/*********************************************************************************************/
/*!
	\brief
		Creates a descriptor set for the texture

	\param window
		The pointer to the game window
*/
/*********************************************************************************************/
void Texture::CreateTextureDescriptorSet(Window* window)
{
	// Creates an info block to allocate the descriptor set structure
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = window->GetDescriptorPool();
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = window->GetTextureDescriptorSetLayout();

	if (vkAllocateDescriptorSets(window->GetLogicalDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = textureImageView;
	imageInfo.sampler = textureSampler;

	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(window->GetLogicalDevice(), 1, &descriptorWrite, 0, NULL);
}

/*************************************************************************************************/
/*!
	\brief
		Determines whether the given filename matches this texture's filename

	\return
		Whether the two filenames are the same
*/
/*************************************************************************************************/
bool operator==(const Texture& lhs, const std::string& rhs)
{
	return !lhs.filename.compare(rhs);
}
bool operator==(const std::string& lhs, const Texture& rhs)
{
	return !rhs.filename.compare(lhs);
}
