/*************************************************************************************************/
/*!
\file RenderPass.cpp
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

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base Includes
#include "RenderPass.h"
#include "cppShortcuts.h"

// Includes file.h so we can read in shaders
#include "File.h"

// Includes vertex so we can draw triangles
#include "Vertex.h"

// Includes the texture class so the render pass can output to a texture
#include "Texture.h"

// Includes the window class so we can communicate the render pass the main window
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
		The constructor for the render pass class
		
	\param
		FILL
		
	\return
		FILL
*/
/*************************************************************************************************/
RenderPass::RenderPass() : singleOutput(false),
	imageViews(0), outputTexture(NULL),
	renderPass(NULL), descriptorSetLayout(NULL), pipelineLayout(NULL), graphicsPipeline(NULL),
	framebuffers(0), uniformBuffers(0), uniformBuffersMemory(0), descriptorSets(0)
{

}

/*************************************************************************************************/
/*!
	\brief
	  Destructor for render pass class
*/
/*************************************************************************************************/
RenderPass::~RenderPass()
{
	
}

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
void RenderPass::CreateTexture(int textureWidth, int textureHeight, VkFormat textureFormat)
{
	// Error check
	if (outputTexture)
	{
		delete outputTexture;
	}

	// Creates the texture
	outputTexture = new Texture(textureWidth, textureHeight, textureFormat);

	// Sets that this render pass outputs to a single texture
	singleOutput = true;
}

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
void RenderPass::CreateRenderPass(bool onScreen, VkDevice vkDevice, VkFormat textureFormat)
{
	// Sets up a single color buffer for the swap chain
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = textureFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	// Sets the reference to the color attachment for subpasses to use
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// Sets up the graphics subpass
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	// Creates the subpass dependency so we can get our image before we start rendering
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcAccessMask = 0;

	// Sets up the render pass
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (onScreen)
	{
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	}
	else
	{
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	}

	// Checks that the render pass was created correctly
	_Window->CheckVulkanSuccess(vkCreateRenderPass(vkDevice, &renderPassInfo, nullptr, &renderPass), "failed to create render pass!");
}

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
void RenderPass::CreateDescriptorSetLayout(VkDevice vkDevice, VkShaderStageFlags shaderStage, std::vector<VkDescriptorType> descriptorTypes)
{
	// Sets the layout for the uniform buffer object
	std::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings;

	for (int i = 0; i < descriptorTypes.size(); i++)
	{
		VkDescriptorSetLayoutBinding newBinding;
		newBinding.binding = i;
		newBinding.descriptorCount = 1;
		newBinding.descriptorType = descriptorTypes[i];
		newBinding.pImmutableSamplers = NULL;
		newBinding.stageFlags = shaderStage;

		uboLayoutBindings.push_back(newBinding);
	}

	// Sets the layout for the descriptor set
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = (uint32_t)uboLayoutBindings.size();
	layoutInfo.pBindings = uboLayoutBindings.data();

	// Makes the offscreen descriptor sets
	_Window->CheckVulkanSuccess(vkCreateDescriptorSetLayout(vkDevice, &layoutInfo, NULL, &descriptorSetLayout), "failed to create descriptor set layout!");
}

/*************************************************************************************************/
/*!
	\brief
		Creates the graphics pipeline for the render pass

	\param vkDevice
		The virtual vulkan device for this render pass to work off of

	\param pipelineCache
		The memory cache where all graphics pipelines are stored

	\param vertexShader
		The vertex shader address for the pipeline

	\param fragmentShader
		The fragment shader address for the pipeline

	\param additionalDescriptorSets
		If there are additional descriptor sets to be added, include them here

	\param pushConstantRange
		The push constant range for the pipeline. Defaults to empty
*/
/*************************************************************************************************/
void RenderPass::CreateGraphicsPipeline(VkDevice& vkDevice, VkPipelineCache& pipelineCache, std::string vertexShader, std::string fragmentShader, std::vector<VkDescriptorSetLayout> additionalDescriptorSets, VkPushConstantRange* pushConstantRange)
{
	// Reads in the shaders
	File vertShaderCode(vertexShader.c_str(), true, false);
	File fragShaderCode(fragmentShader.c_str(), true, false);

	// Converts the raw data into the shader modules
	VkShaderModule vertShaderModule = CreateShaderModule(vkDevice, vertShaderCode.GetReadData());
	VkShaderModule fragShaderModule = CreateShaderModule(vkDevice, fragShaderCode.GetReadData());

	// Creates the vertex shader stage information
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	// Creates the fragment shader stage information
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	// Saves the shader stages in an array
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	// Gets the vertex description structs
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	// Tells the graphics pipeline to use a list of individual triangles
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// Tells the graphics pipeline how many viewports and scissor windows will exist
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	// Sets up the rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	// Explicitly turns off multisampling
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;										// DIFFERS BETWEEN TUTORIALS!!!!!!!!

	// Sets the color blend attachment
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	// Sets up the color blend constants (currently no blending)
	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	// The list of things that should be dynamic
	std::vector<VkDynamicState> dynamicStates =
	{
	VK_DYNAMIC_STATE_VIEWPORT,
	VK_DYNAMIC_STATE_SCISSOR
	};

	// Tells the pipeline to ignore statically determining those aspects
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	// Creates the pipeline layout 
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	additionalDescriptorSets.insert(additionalDescriptorSets.begin(), descriptorSetLayout);
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(additionalDescriptorSets.size());
	pipelineLayoutInfo.pSetLayouts = additionalDescriptorSets.data();
	if (pushConstantRange)
	{
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = pushConstantRange;
	}
	_Window->CheckVulkanSuccess(vkCreatePipelineLayout(vkDevice, &pipelineLayoutInfo, NULL, &pipelineLayout), "failed to create offscreen pipeline layout");

	// Sets the vertex description structs
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	// Creates the graphics pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	_Window->CheckVulkanSuccess(vkCreateGraphicsPipelines(vkDevice, pipelineCache, 1, &pipelineInfo, NULL, &graphicsPipeline), "failed to create offscreen graphics pipeline");

	// Cleans up the shader modules
	vkDestroyShaderModule(vkDevice, fragShaderModule, NULL);
	vkDestroyShaderModule(vkDevice, vertShaderModule, NULL);
}

/*************************************************************************************************/
/*!
	\brief
		Assigns the given swap chain framebuffers to the render pass

	\param vkDevice
		The virtual vulkan device for this render pass to work off of

	\param swapChainFramebuffers
		The framebuffers for the window's swap chain system

	\param swapChainExtent
		The extent of the framebuffers
*/
/*************************************************************************************************/
void RenderPass::SetSwapChainFramebuffers(VkDevice& vkDevice, std::vector<VkFramebuffer>& swapChainFramebuffers, VkExtent2D swapChainExtent)
{
	// Sets the number of frame buffers
	swapChainFramebuffers.resize(imageViews.size());

	// Makes a frame buffer for each slot in the vector
	for (size_t i = 0; i < imageViews.size(); i++)
	{
		// Creates the framebuffer info struct do set the details of the swap chain framebuffers
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = &imageViews[i];
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		_Window->CheckVulkanSuccess(vkCreateFramebuffer(vkDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]), "failed to create framebuffer!");
	}
}

/*************************************************************************************************/
/*!
	\brief
		Creates the framebuffers for the render pass

	\param vkDevice
		The virtual vulkan device for this render pass to work off of

	\param swapChainExtent
		The extent of the framebuffers
*/
/*************************************************************************************************/
void RenderPass::SetFramebuffers(VkDevice& vkDevice, VkExtent2D extent)
{
	// Sets the number of frame buffers																!!! Currently Hardcoded to 1
	framebuffers.resize(1);

	// Creates the framebuffer info struct do set the details of the swap chain framebuffers
	VkFramebufferCreateInfo framebufferInfo{};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = renderPass;
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.pAttachments = &outputTexture->GetImageView();
	framebufferInfo.width = extent.width;
	framebufferInfo.height = extent.height;
	framebufferInfo.layers = 1;

	_Window->CheckVulkanSuccess(vkCreateFramebuffer(vkDevice, &framebufferInfo, nullptr, &framebuffers[0]), "failed to create framebuffer!");
}

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
void RenderPass::CreateUniformBuffers(VkDevice& vkDevice, VkDeviceSize bufferSize, int bufferQuantity)
{
	// Sets there to be a buffer for each framebuffer
	uniformBuffers.resize(bufferQuantity);
	uniformBuffersMemory.resize(bufferQuantity);

	// Creates the uniform buffer objects
	for (size_t i = 0; i < bufferQuantity; i++)
	{
		_Window->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

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
void RenderPass::CreateDescriptorSet(VkDevice& vkDevice, VkDescriptorPool& descriptorPool, int descriptorSetQuantity)
{
	// Sets the info struct for the descriptor set
	std::vector<VkDescriptorSetLayout> layouts(descriptorSetQuantity, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSetQuantity);
	allocInfo.pSetLayouts = layouts.data();

	// Allocates the memory for the descriptor sets
	descriptorSets.resize(descriptorSetQuantity);
	_Window->CheckVulkanSuccess(vkAllocateDescriptorSets(vkDevice, &allocInfo, descriptorSets.data()), "failed to allocate descriptor sets!");
}

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
void RenderPass::UpdateDescriptorSetUniformBuffer(VkDevice& vkDevice, int bufferSize)
{
	// Initializes the data for each descriptor set
	for (size_t i = 0; i < descriptorSets.size(); i++)
	{
		// Specifies the buffer size
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = bufferSize;

		// Creates an array for both descriptor types
		VkWriteDescriptorSet descriptorWrite{};

		// Sets the properties of the ubo descriptor
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		// Updates the descriptor set
		vkUpdateDescriptorSets(vkDevice, 1, &descriptorWrite, 0, NULL);
	}
}

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
void RenderPass::UpdateUniformBuffer(VkDevice& vkDevice, int bufferSize, void* uniformBuffer, int currentFrame)
{
	// Copies all the uniform buffer data in
	void* data;
	vkMapMemory(vkDevice, uniformBuffersMemory[currentFrame], 0, bufferSize, 0, &data);
	memcpy(data, uniformBuffer, bufferSize);
	vkUnmapMemory(vkDevice, uniformBuffersMemory[currentFrame]);
}

/*************************************************************************************************/
/*!
	\brief
		Destroys the output texture of the render pass (if there is one). Otherwise destroys the image view object.

	\param vkDevice
		The virtual vulkan device for this render pass to work off of
*/
/*************************************************************************************************/
void RenderPass::DestroyTexture(VkDevice& vkDevice)
{
	// If there's an output texture
	if (outputTexture)
	{
		// Destroys the texture object
		delete outputTexture;
		outputTexture = NULL;
	}
	// Otherwise
	else
	{
		// Clears the image view object
		// Destroys all the image views
		for (size_t i = 0; i < imageViews.size(); i++)
		{
			vkDestroyImageView(vkDevice, imageViews[i], nullptr);
		}

		imageViews.clear();
	}
}

/*************************************************************************************************/
/*!
	\brief
		Destroys the render pass

	\param vkDevice
		The virtual vulkan device for this render pass to work off of
*/
/*************************************************************************************************/
void RenderPass::DestroyRenderPass(VkDevice& vkDevice)
{
	// Cleans up the render pass, pipeline layout, and pipeline variables
	vkDestroyPipeline(vkDevice, graphicsPipeline, NULL);
	vkDestroyPipelineLayout(vkDevice, pipelineLayout, NULL);
	vkDestroyRenderPass(vkDevice, renderPass, NULL);

	// Cleans up the uniform buffer objects
	for (size_t i = 0; i < uniformBuffers.size(); i++)
	{
		vkDestroyBuffer(vkDevice, uniformBuffers[i], NULL);
		vkFreeMemory(vkDevice, uniformBuffersMemory[i], NULL);
	}

	// Cleans up the uniform buffer objects
	for (size_t i = 0; i < framebuffers.size(); i++)
	{
		vkDestroyFramebuffer(vkDevice, framebuffers[i], NULL);
	}

	// Destroys the descriptor sets
	vkDestroyDescriptorSetLayout(vkDevice, descriptorSetLayout, NULL);
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*********************************************************************************************/
/*!
	\brief
		Formats shaders for use

	\param vkDevice
		The virtual vulkan device

	\param code
		The raw shader code

	\return
		The resultant shader module
*/
/*********************************************************************************************/
VkShaderModule RenderPass::CreateShaderModule(VkDevice vkDevice, const std::vector<char>& code)
{
	// Converts the raw read in data into a the shader module info struct
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	// The shader module
	VkShaderModule shaderModule;

	// Creates and checks that the shader module was created correctly
	_Window->CheckVulkanSuccess(vkCreateShaderModule(vkDevice, &createInfo, nullptr, &shaderModule), "failed to create shader module!");

	// Returns the shader module
	return shaderModule;
}
