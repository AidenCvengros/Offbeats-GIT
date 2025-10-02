/*************************************************************************************************/
/*!
\file RenderPass.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2025.8.29
\brief
    Handles render pass data

    Functions include:
        + FILL

Copyright (c) 2025 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_RenderPass_H_
#define Syncopatience_RenderPass_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base include
#include "stdafx.h"

// Includes glfw libraries for callback functions
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Includes vectors for lists of input variables
#include <vector>

//-------------------------------------------------------------------------------------------------
// Forward References
//-------------------------------------------------------------------------------------------------

// Dependency Reference
class Texture;

//-------------------------------------------------------------------------------------------------
// Public Constants
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Classes
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		The render pass class
*/
/*************************************************************************************************/
class RenderPass
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
		  Constructor for the render pass class
	*/
	/*************************************************************************************************/
	RenderPass();
	
	/*************************************************************************************************/
	/*!
		\brief
		  Destructor for render pass class
	*/
	/*************************************************************************************************/
	~RenderPass();

	/*************************************************************************************************/
	/*!
		\brief
			Creates the texture the render pass is writing to. If this function is not called, the render pass will not write to a texture

		\param textureWidth
			The width of the texture

		\param textureHeight
			The height of the texture

		\param textureFormat
			The color format for the texture
	*/
	/*************************************************************************************************/
	void CreateTexture(int textureWidth, int textureHeight, VkFormat textureFormat);

	/*************************************************************************************************/
	/*!
		\brief
			Creates the vulkan render pass object

		\param onScreen
			Whether this render pass is outputting to the screen or not

		\param vkDevice
			The virtual vulkan device for this render pass to work off of

		\param textureFormat
			The color format for the texture
	*/
	/*************************************************************************************************/
	void CreateRenderPass(bool onScreen, VkDevice vkDevice, VkFormat textureFormat);

	/*************************************************************************************************/
	/*!
		\brief
			Creates the descriptor set layout for the render pass

		\param vkDevice
			The virtual vulkan device for this render pass to work off of

		\param shaderStage
			What stage the descriptor set layout is being created for

		\param descriptorTypes
			The list of descriptor types in the set layout
	*/
	/*************************************************************************************************/
	void CreateDescriptorSetLayout(VkDevice vkDevice, VkShaderStageFlags shaderStage, std::vector<VkDescriptorType> descriptorTypes);

	/*************************************************************************************************/
	/*!
		\brief
			Creates the graphics pipeline for the render pass

		\param vkDevice
			The virtual vulkan device for this render pass to work off of

		\param pipelineCache
			The memory cache that stores all pipelines

		\param vertexShader
			The vertex shader address for the pipeline

		\param fragmentShader
			The fragment shader address for the pipelin

		\param additionalDescriptorSets
			If there are additional descriptor sets to be added, include them here

		\param pushConstantRange
			The push constant range for the pipeline. Defaults to empty
	*/
	/*************************************************************************************************/
	void CreateGraphicsPipeline(VkDevice& vkDevice, VkPipelineCache& pipelineCache, std::string vertexShader, std::string fragmentShader, std::vector<VkDescriptorSetLayout> additionalDescriptorSets, VkPushConstantRange* pushConstantRange = NULL);

	/*************************************************************************************************/
	/*!
		\brief
			Creates the swap chain framebuffers (only needed if this render pass interacts with the swap chain)

		\param vkDevice
			The virtual vulkan device for this render pass to work off of

		\param swapChainFramebuffers
			The framebuffers for the window's swap chain system

		\param swapChainExtent
			The extent of the framebuffers
	*/
	/*************************************************************************************************/
	void SetSwapChainFramebuffers(VkDevice& vkDevice, std::vector<VkFramebuffer>& swapChainFramebuffers, VkExtent2D swapChainExtent);

	/*************************************************************************************************/
	/*!
		\brief
			Creates the framebuffers for this render pass

		\param vkDevice
			The virtual vulkan device for this render pass to work off of

		\param extent
			The extent of the framebuffers
	*/
	/*************************************************************************************************/
	void SetFramebuffers(VkDevice& vkDevice, VkExtent2D extent);

	/*************************************************************************************************/
	/*!
		\brief
			Creates the framebuffers for this render pass

		\param vkDevice
			The virtual vulkan device for this render pass to work off of

		\param bufferSize
			The size of the uniform buffer object

		\param bufferQuantity
			The number of uniform buffers to create
	*/
	/*************************************************************************************************/
	void CreateUniformBuffers(VkDevice& vkDevice, VkDeviceSize bufferSize, int bufferQuantity);

	/*************************************************************************************************/
	/*!
		\brief
			Creates the descriptor set for this render pass

		\param vkDevice
			The virtual vulkan device for this render pass to work off of

		\param descriptorPool
			The descriptor pool the descriptor set is in

		\param descriptorSetQuantity
			How many frames worth of descriptor sets to create
	*/
	/*************************************************************************************************/
	void CreateDescriptorSet(VkDevice& vkDevice, VkDescriptorPool& descriptorPool, int descriptorSetQuantity);

	/*************************************************************************************************/
	/*!
		\brief
			Updates the uniform buffer for the descriptor set for this render pass (sets the uniform buffer to binding 0)

		\param vkDevice
			The virtual vulkan device for this render pass to work off of

		\param bufferSize
			The size of the uniform buffer
	*/
	/*************************************************************************************************/
	void UpdateDescriptorSetUniformBuffer(VkDevice& vkDevice, int bufferSize);

	/*************************************************************************************************/
	/*!
		\brief
			Updates the uniform buffer for the descriptor set for this render pass (sets the uniform buffer to binding 0)

		\param vkDevice
			The virtual vulkan device for this render pass to work off of

		\param bufferSize
			The size of the uniform buffer

		\param uniformBuffer
			The uniform buffer data. Uses a void pointer because different uniform buffers are different structs

		\param currentFrame
			The frame that is being calculated. Defaults to zero for render passes that don't have multiple frames set up.
	*/
	/*************************************************************************************************/
	void UpdateUniformBuffer(VkDevice& vkDevice, int bufferSize, void* uniformBuffer, int currentFrame = 0);

	/*************************************************************************************************/
	/*!
		\brief
			Destroys the output texture of the render pass (if there is one). Otherwise destroys the image view object.

		\param vkDevice
			The virtual vulkan device for this render pass to work off of
	*/
	/*************************************************************************************************/
	void DestroyTexture(VkDevice& vkDevice);

	/*************************************************************************************************/
	/*!
		\brief
			Destroys the render pass

		\param vkDevice
			The virtual vulkan device for this render pass to work off of
	*/
	/*************************************************************************************************/
	void DestroyRenderPass(VkDevice& vkDevice);

	/*************************************************************************************************/
	/*!
		\brief
			Adds the given image view object for the render pass
	*/
	/*************************************************************************************************/
	void AddImageView(VkImageView newImageView) { imageViews.push_back(newImageView); }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the uniform buffers for this render pass

		\return
			The uniform buffers of the render pass
	*/
	/*************************************************************************************************/
	std::vector<VkBuffer> GetUniformBuffers() { return uniformBuffers; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the texture object this render pass outputs to

		\return
			The output texture object
	*/
	/*************************************************************************************************/
	Texture* GetOutputTexture() { return outputTexture; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the vulkan render pass object

		\return
			The vulkan render pass object
	*/
	/*************************************************************************************************/
	VkRenderPass& GetRenderPass() { return renderPass; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the render pass framebuffers

		\return
			The list of framebuffers
	*/
	/*************************************************************************************************/
	std::vector<VkFramebuffer>& GetFramebuffers() { return framebuffers; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the render pass graphics pipeline object

		\return
			The graphics pipeline object
	*/
	/*************************************************************************************************/
	VkPipeline& GetGraphicsPipeline() { return graphicsPipeline; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the render pass graphics pipeline layout object

		\return
			The graphics pipeline layout object
	*/
	/*************************************************************************************************/
	VkPipelineLayout& GetGraphicsPipelineLayout() { return pipelineLayout; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the render pass descriptor sets

		\return
			The descriptor sets
	*/
	/*************************************************************************************************/
	std::vector<VkDescriptorSet>& GetDescriptorSets() { return descriptorSets; }
	
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

	bool singleOutput;									// Whether this render pass always outputs to a single output (outputTexture) or multiple (imageViews)
	std::vector<VkImageView> imageViews;				// The image view is the part in memory that visuals get written to
	Texture* outputTexture;								// Output texture. Will be null if this render pass outputs to screen
	VkRenderPass renderPass;							// The render pass for the offscreen buffers
	VkDescriptorSetLayout descriptorSetLayout;			// The descriptor set layout for the fisheye shader
	VkPipelineLayout pipelineLayout;					// The graphics pipeline layout for the fisheye render pass
	VkPipeline graphicsPipeline;						// The graphics pipeline for the fisheye render pass
	std::vector<VkFramebuffer> framebuffers;			// The offscreen framebuffer
	std::vector<VkBuffer> uniformBuffers;				// The uniform buffer for the fisheye shader
	std::vector<VkDeviceMemory> uniformBuffersMemory;	// The memory storing unform buffer data for the fisheye shader
	std::vector<VkDescriptorSet> descriptorSets;		// The descriptor set for the fisheye shader
	
	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

	/*********************************************************************************************/
	/*!
		\brief
			Formats shaders for use

		\param vkDevice
			The virtual vulkan device being used

		\param code
			The raw shader code

		\return
			The resultant shader module
	*/
	/*********************************************************************************************/
	VkShaderModule CreateShaderModule(VkDevice vkDevice, const std::vector<char>& code);
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_RenderPass_H_
