/*************************************************************************************************/
/*!
\file Window.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2023.9.7
\brief
    Opens and manages the program's Vulkan window.

    Public Functions:
		+ Window::~Window
		+ Window::Init
		+ Window::Update
		+ Window::Draw
		+ Window::Shutdown

	Private Functions:

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base Includes
#include "Window.h"
#include "cppShortcuts.h"

// Includes set for making lists
#include <set>

// Includes for mathing
#include <algorithm>

// Functionality for running and drawing to the window
#include "Vertex.h"

// Game Object object to keep track of things to draw to the screen
#include "../Game_Objects/GameObject.h"

// Includes the texture class for the default blank texture
#include "Texture.h"

// Includes the camera class so we can get the perspective matrix
#include "../Game_Objects/Camera.h"

//-------------------------------------------------------------------------------------------------
// Private Constants
//-------------------------------------------------------------------------------------------------

// The default triangle
static const std::vector<Vertex> defaultTri =
{
	{{0.0f, -0.5f}, {1.0f, 0.2f, 0.2f}, {1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.5f}, {0.5f, 1.0f}},
	{{-0.5f, 0.5f}, {0.0f, 0.5f, 1.0f}, {1.0f, 1.0f}}
};

// The default rectangle
static const std::vector<Vertex> defaultRect =
{
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

// The indices for points on the default rectangle
const std::vector<uint16_t> defaultRectIndices = { 0, 1, 2, 2, 3, 0 };

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

/*********************************************************************************************/
/*!
	\brief
		Deconstructor for the window
*/
/*********************************************************************************************/
Window::~Window()
{

}

/*********************************************************************************************/
/*!
	\brief
		Starts the window and runs all setup functions for starting Vulkan on said window
*/
/*********************************************************************************************/
void Window::Init()
{
	// Sets up glfw
	glfwInit();

	// Tells glfw that we aren't using OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Creates the window
	window = glfwCreateWindow(width, height, name.c_str() , NULL, NULL);

	// Sets the window pointer and resize callback function
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);

	// Initializes the Vulkan instance
	CreateVulkanInstance();
	SetupDebugMessenger();
	InitializeSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateSwapChain();
	CreateCommandPool();
	CreateDescriptorPool();
	CreateDescriptorSetLayout();
	CreateImageViews();
	CreateRenderPass();
	CreateGraphicsPipeline();
	CreateFramebuffers();
	CreateUniformBuffers();
	CreateDescriptorSets();
	CreateSyncObjects();

	// Creates a blank texture as a default option for objects without sprites
	blankTexture = new Texture("Assets/Sprites/Blank.png");
}

/*********************************************************************************************/
/*!
	\brief
		Update function for the window

	\brief
		The time elapsed since the previous frame
*/
/*********************************************************************************************/
void Window::Update(double dt)
{
	// Updates window checks
	glfwPollEvents();
}

/*********************************************************************************************/
/*!
	\brief
		Draws the window
*/
/*********************************************************************************************/
void Window::Draw()
{
	// Makes sure that the previous frame has finished before drawing the next one
	vkWaitForFences(logicalDevice, 1, &inFlightFence[currentFrame], VK_TRUE, UINT64_MAX);

	// Gets the next image from the swap chain
	VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain, UINT64_MAX, availableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

	// Checks if the swap chain needs to be rebuilt or if something went wrong with the new image
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	// Updates the uniform buffer matrices
	UpdateUniformBuffers(currentFrame);

	// Sets a new fence for synchronization
	vkResetFences(logicalDevice, 1, &inFlightFence[currentFrame]);
	// Starts the new command buffer (follow here for draw command)
	vkResetCommandBuffer(commandBuffer[currentFrame], 0);
	SetupCommandBuffer(commandBuffer[currentFrame], imageIndex);

	// Sets the info for the render pass
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = baseScenePass.GetRenderPass();
	renderPassInfo.framebuffer = baseScenePass.GetFramebuffers()[0];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	// Sets the render size
	VkClearValue clearColor = { {{0.05f, 0.5f, 0.8f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	// Start of the render pass
	vkCmdBeginRenderPass(commandBuffer[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Binds the render pass
	vkCmdBindPipeline(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, baseScenePass.GetGraphicsPipeline());

	// Sets the viewport for the render pass
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer[currentFrame], 0, 1, &viewport);

	// Sets the scissor state for the render pass
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer[currentFrame], 0, 1, &scissor);

	// Binds the descriptor sets
	vkCmdBindDescriptorSets(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, baseScenePass.GetGraphicsPipelineLayout(), 0, 1, &baseScenePass.GetDescriptorSets()[0], 0, NULL);
}

/*********************************************************************************************/
/*!
	\brief
		Begins the mask render pass drawing sequence
*/
/*************************************************************************************************/
void Window::DrawMaskRenderPass()
{
	// Ends the previous render pass
	vkCmdEndRenderPass(commandBuffer[currentFrame]);

	// Sets the info for the render pass
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = glitchMaskPass.GetRenderPass();
	renderPassInfo.framebuffer = glitchMaskPass.GetFramebuffers()[0];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	// Sets the render size
	VkClearValue clearColor = { {1.0f, 1.0f, 1.0f, 0.0f} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	// Start of the render pass
	vkCmdBeginRenderPass(commandBuffer[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Binds the render pass
	vkCmdBindPipeline(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, glitchMaskPass.GetGraphicsPipeline());

	// Binds the descriptor sets
	vkCmdBindDescriptorSets(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, glitchMaskPass.GetGraphicsPipelineLayout(), 0, 1, &glitchMaskPass.GetDescriptorSets()[0], 0, NULL);
}

/*********************************************************************************************/
/*!
	\brief
		Draws the given game object

	\param transform
		The given object to be drawn
*/
/*********************************************************************************************/
void Window::DrawGameObject(GameObject* gameObject)
{
	// If the game object is supposed to be rendered
	if (gameObject->GetRender())
	{
		// Sets the dynamic offset and draws the first object
		glm::mat4 tempMat = gameObject->GetTranformationMatrix();
		glm::vec4 tempColor = gameObject->GetColor();
		vkCmdPushConstants(commandBuffer[currentFrame], baseScenePass.GetGraphicsPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &tempMat);
		vkCmdPushConstants(commandBuffer[currentFrame], baseScenePass.GetGraphicsPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 64, sizeof(glm::vec4), &tempColor);

		// Checks if the game object has a texture
		if (gameObject->GetTexture())
		{
			// Binds the texture descriptor set and color
			vkCmdBindDescriptorSets(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, baseScenePass.GetGraphicsPipelineLayout(), 1, 1, gameObject->GetTexture()->GetDescriptorSet(), 0, NULL);
		}
		// Otherwise uses the default blank texture
		else
		{
			vkCmdBindDescriptorSets(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, baseScenePass.GetGraphicsPipelineLayout(), 1, 1, blankTexture->GetDescriptorSet(), 0, NULL);
		}

		// Records the draw command to the command buffer
		RecordCommandBuffer(commandBuffer[currentFrame]);
	}
}

/*********************************************************************************************/
/*!
	\brief
		Ends the drawing sequence
*/
/*********************************************************************************************/
void Window::CleanupDraw()
{
	// Ends the render pass
	vkCmdEndRenderPass(commandBuffer[currentFrame]);

	// Runs the post processing shaders
	RunFisheyeRenderPass();

	// Checks that everything happened correctly
	CheckVulkanSuccess(vkEndCommandBuffer(commandBuffer[currentFrame]), "failed to record command buffer!");

	// Makes the queue submission info struct
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	// Sets up the available semaphore
	VkSemaphore waitSemaphores[] = { availableSemaphore[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	// Gives the graphics pipeline the command buffer
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer[currentFrame];

	// Sets up the finished semaphore
	VkSemaphore signalSemaphores[] = { finishedSemaphore[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	// Submits the command buffer to the graphics queue
	CheckVulkanSuccess(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence[currentFrame]), "failed to submit draw command buffer!");

	// Creates the presentation struct
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	// Submits the image to the swap chain
	VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);

	// Checks if the window size is out of date
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
	{
		// Resizes the window appropriately
		framebufferResized = false;
		RecreateSwapChain();
	}
	// Checks if the swap chain update failed somehow
	CheckVulkanSuccess(result, "failed to present swap chain image!");

	// Increments the current frame index
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

/*********************************************************************************************/
/*!
	\brief
		Shuts down the window
*/
/*********************************************************************************************/
void Window::Shutdown()
{
	// Waits for any existing draw operations to be finished
	vkDeviceWaitIdle(logicalDevice);

	// Deletes the blank texture
	delete blankTexture;

	// Cleans up the swap chain
	CleanupSwapChain();

	// Destroys the render passes
	baseScenePass.DestroyRenderPass(logicalDevice);
	glitchMaskPass.DestroyRenderPass(logicalDevice);
	postProcessPass.DestroyRenderPass(logicalDevice);

	// Cleans up the render pass, pipeline layout, and pipeline variables
	vkDestroyPipelineCache(logicalDevice, pipelineCache, NULL);

	// Destroys the descriptor sets
	vkDestroyDescriptorPool(logicalDevice, descriptorPool, NULL);
	vkDestroyDescriptorSetLayout(logicalDevice, textureDescriptorSetLayout, NULL);

	// Destroys the index buffer
	vkDestroyBuffer(logicalDevice, indexBuffer, NULL);
	vkFreeMemory(logicalDevice, indexBufferMemory, NULL);

	// Destroys the vertex buffer
	vkDestroyBuffer(logicalDevice, vertexBuffer, NULL);
	vkFreeMemory(logicalDevice, vertexBufferMemory, NULL);

	// Loops to destroy the semaphores for each active frame
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		// Destroys the synchronization objects
		vkDestroySemaphore(logicalDevice, availableSemaphore[i], NULL);
		vkDestroySemaphore(logicalDevice, finishedSemaphore[i], NULL);
		vkDestroyFence(logicalDevice, inFlightFence[i], NULL);
	}

	// Destroys the command pool
	vkDestroyCommandPool(logicalDevice, commandPool, NULL);

	// Destroys the Vulkan instance
	vkDestroyDevice(logicalDevice, NULL);
	
	// Destroys the validation layers
	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(vulkanInstance, debugMessenger, NULL);
	}

	// Destroys the Vulkan Instance
	vkDestroySurfaceKHR(vulkanInstance, surface, NULL);
	vkDestroyInstance(vulkanInstance, NULL);

	// Destroys the window and turns off glfw
	glfwDestroyWindow(window);
	glfwTerminate();
}

/*********************************************************************************************/
/*!
	\brief
		The callback function for debug messages

	\param messageSeverity
		How important the debug message is

	\param messageType
		The type of the message

	\param pCallbackData
		Debug data

	\param pUserData
		User data

	\return
		Whether the message was handled
*/
/*********************************************************************************************/
VKAPI_ATTR VkBool32 VKAPI_CALL Window::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	// Reads out the error message
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	// Returns that nothing was done with the message
	return VK_FALSE;
}

/*********************************************************************************************/
/*!
	\brief
		The callback function for when the window is resized

	\param window_
		Pointer to the window

	\param width
		The new width of the window

	\param height
		The new height of the window
*/
/*********************************************************************************************/
void Window::FramebufferResizeCallback(GLFWwindow* window_, int width, int height)
{
	// Creates a new window pointer
	Window* app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window_));

	// Sets the boolean to update window related objects
	app->framebufferResized = true;
}

/*********************************************************************************************/
/*!
	\brief
		Starts a command buffer

	\return
		The new command buffer
*/
/*********************************************************************************************/
VkCommandBuffer Window::BeginSingleTimeCommands() const
{
	// Info block that gives vulkan the command pool
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	// Allocates the command buffer
	VkCommandBuffer commandBuffer_;
	vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer_);

	// Marks that this command buffer is just for this frame
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	// Starts using the command buffer
	vkBeginCommandBuffer(commandBuffer_, &beginInfo);
	return commandBuffer_;
}

/*********************************************************************************************/
/*!
	\brief
		Ends the given command buffer

	\param commandBuffer_
		The command buffer to be ended
*/
/*********************************************************************************************/
void Window::EndSingleTimeCommands(VkCommandBuffer commandBuffer_) const
{
	// Stops using the command buffer
	vkEndCommandBuffer(commandBuffer_);

	// Info block to wait for semaphore
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer_;

	// Waits for the graphics queue to be finished
	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	// Once commands are done, we can free the command buffer
	vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer_);
}

/*********************************************************************************************/
/*!
	\brief
		Waits for all draw commands to finish
*/
/*********************************************************************************************/
void Window::WaitForDrawFinished()
{
	// Ends the render pass
	vkQueueWaitIdle(graphicsQueue);
}

/*********************************************************************************************/
/*!
	\brief
		Checks if the given VkResult is VkSuccess. Otherwise throws an error with the given message.

	\param functionResult
		The result of a vulkan function

	\param errorMessage
		The error message to print.

	\return
		Returns the given VkResult
*/
/*********************************************************************************************/
VkResult Window::CheckVulkanSuccess(VkResult functionResult, std::string errorMessage)
{
	// Checks if the function failed
	if (functionResult != VK_SUCCESS)
	{
		// If it did, throws the error message
		throw std::runtime_error(errorMessage + " Error code: " + std::to_string(functionResult));
	}

	// Otherwise passes along the function result
	return functionResult;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

//// Initialization Functions

/*********************************************************************************************/
/*!
	\brief
		Initializes Vulkan
*/
/*********************************************************************************************/
void Window::CreateVulkanInstance()
{
	// Defines the application info
	VkApplicationInfo applicationInfo{};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "Retrofit";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);									// !!! Needs accurate game version !!!
	applicationInfo.pEngineName = "Custom Engine made by Aiden Cvengros";
	applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);										// !!! Needs accurate engine version !!!
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	// Sets the information struct for creating the Vulkan instance
	VkInstanceCreateInfo vulkanInstanceInfo{};
	vulkanInstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vulkanInstanceInfo.pApplicationInfo = &applicationInfo;
	auto extensions = GetRequiredExtensions();
	vulkanInstanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	vulkanInstanceInfo.ppEnabledExtensionNames = extensions.data();

	// Checks that validation layers can be turned on if they will need to be
	if (enableValidationLayers && !CheckValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}

	// Turns validation layers on or off accordingly
	if (enableValidationLayers)
	{
		// Adds the validation layers to the vulkan instance
		vulkanInstanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		vulkanInstanceInfo.ppEnabledLayerNames = validationLayers.data();

		// Creates the messaging system for debug output
		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo{};
		populateDebugMessengerCreateInfo(debugMessengerInfo);
		vulkanInstanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;
	}
	else
	{
		// If validation layers are off, clears the relevant fields for the vulkan instance
		vulkanInstanceInfo.enabledLayerCount = 0;
		vulkanInstanceInfo.pNext = NULL;
	}

	// Creates the vulkan instance
	CheckVulkanSuccess(vkCreateInstance(&vulkanInstanceInfo, NULL, &vulkanInstance), "Failed to create Vulkan instance!");
}

/*********************************************************************************************/
/*!
	\brief
		Initializes the debug messenger
*/
/*********************************************************************************************/
void Window::SetupDebugMessenger()
{
	// If we aren't doing validation layers, skip this function
	if (!enableValidationLayers) return;

	// Makes an info stucture for the message
	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	// Creates the debug messenger
	CheckVulkanSuccess(CreateDebugUtilsMessengerEXT(vulkanInstance, &createInfo, NULL, &debugMessenger), "Failed to create debug messenger!");
}

/*********************************************************************************************/
/*!
	\brief
		Initializes the virtual screen surface
*/
/*********************************************************************************************/
void Window::InitializeSurface()
{
	// Creates the window surface object
	CheckVulkanSuccess(glfwCreateWindowSurface(vulkanInstance, window, NULL, &surface), "Failed to initialize virtual screen surface");
}

/*********************************************************************************************/
/*!
	\brief
		Picks a graphics (or other valid) card for Vulkan to render on
*/
/*********************************************************************************************/
void Window::PickPhysicalDevice()
{
	// Queries how many devices are available
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, NULL);

	// Fails if no cards were found
	if (deviceCount == 0)
	{
		throw std::runtime_error("No graphics devices were found");
	}

	// Makes a vector list of all the devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, devices.data());

	// Checks for a suitable device
	for (const auto& device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			physicalCard = device;
			break;
		}
	}

	// If there isn't a valid graphics card, throws an error
	if (physicalCard == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find physical graphics device");
	}
}

//// Helper Functions

/*********************************************************************************************/
/*!
	\brief
		Makes the info struct for debug messaging systems

	\param createInfo
		The output info struct
*/
/*********************************************************************************************/
void Window::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	// Sets the class members for the debug messenger
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

/*********************************************************************************************/
/*!
	\brief
		Assigns our messaging system as the debug utility messenger

	\param instance
		The Vulkan instance

	\param pCreateInfo,
		The messenger info struct

	\param pAllocator
		the allocator callback function

	\param pDebugMessenger
		The messenger

	\result
		Whether the debug utility messenger exists
*/
/*********************************************************************************************/
VkResult Window::CreateDebugUtilsMessengerEXT(
	VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	// Gets the function that adds in debug utilities messengers
	PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != NULL)
	{
		// If we got a function to do so, adds in our debug messenger
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		// Otherwise returns an error
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

/*********************************************************************************************/
/*!
	\brief
		Determines whether the given graphics card is suitable for running the game

	\param device_
		The given graphics card

	\return
		Whether the graphics card is suitable
*/
/*********************************************************************************************/
bool Window::IsDeviceSuitable(VkPhysicalDevice device_)
{
	// Destroys each of the frame buffers
	for (auto framebuffer : swapChainFramebuffers)
	{
		vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
	}

	// Finds if there is a valid device queue
	QueueFamilyIndices indices = FindQueueFamilies(device_);

	// Checks whether the necessary extensions are supported by the device
	bool extensionsSupported = CheckDeviceExtensionSupport(device_);

	// Checks if swap chain is suitable for the game
	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device_);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	// Gets the physical device's features
	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device_, &supportedFeatures);

	// If all of that is true, returns true
	return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

/*********************************************************************************************/
/*!
	\brief
		Finds what types of queues the graphics card supports that we want to use

	\param device_
		The given graphics card

	\return
		The types of queue families for our program to use
*/
/*********************************************************************************************/
Window::QueueFamilyIndices Window::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;					// Creates the return struct that tracks the different types of queue families

	// Finds how many queue types the device supports, then finds the full list of those types
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	// Loops through the queue type list to find the queue families we want
	int queueTypeIndex = 0;
	for (const VkQueueFamilyProperties& queueFamily : queueFamilies)
	{
		// Checks if the current queue type supports graphics
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = queueTypeIndex;
		}

		// Gets whether the device can render a surface
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, queueTypeIndex, surface, &presentSupport);

		// If the device can render a surface, save it
		if (presentSupport)
		{
			indices.presentFamily = queueTypeIndex;
		}

		// Checks if we've found both graphics and presentation queue support
		if (indices.IsComplete())
		{
			break;
		}

		// Otherwise continues down the list
		queueTypeIndex++;
	}

	// Returns the queue family indices struct
	return indices;
}

/*********************************************************************************************/
/*!
	\brief
		Determines whether the given graphics card has the extensions that the program needs.

	\param device
		the given graphics card

	\return
		Returns true if the device has the necessary extensions
*/
/*********************************************************************************************/
bool Window::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	// Gets the total number of available extensions
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);

	// Gets the extensions
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensions.data());

	// Gets a list of the different extensions we need
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	// Checks if all of those extensions are present
	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	// If the device has all the extensions, returns true
	return requiredExtensions.empty();
}

/*********************************************************************************************/
/*!
	\brief
		Gets the details of the swap chain

	\param device
		the given graphics card

	\return
		The swap chain details
*/
/*********************************************************************************************/
Window::SwapChainSupportDetails Window::QuerySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;			// Holds the swap chain details

	// Gets the basic surface capabilities
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	// Gets the list of surface formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	// Gets the list of presentation modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	// Returns the swap chain details
	return details;
}

//// Destruction Functions

/*********************************************************************************************/
/*!
	\brief
		Remakes the swap chain if it needs changes
*/
/*********************************************************************************************/
void Window::RecreateSwapChain()
{
	// Waits for the previous frame to finish drawing
	vkDeviceWaitIdle(logicalDevice);

	// Cleans up the old swap chain
	CleanupSwapChain();

	// Remakes the swap chain, image views, and frame buffers
	CreateSwapChain();
	CreateImageViews();
	CreateFramebuffers();
	UpdateDescriptorSets();
}

/*********************************************************************************************/
/*!
	\brief
		Destroys the swap chain
*/
/*********************************************************************************************/
void Window::CleanupSwapChain()
{
	// Destroys the texture parts of the render passes
	baseScenePass.DestroyTexture(logicalDevice);
	glitchMaskPass.DestroyTexture(logicalDevice);
	postProcessPass.DestroyTexture(logicalDevice);

	// Destroys all the frame buffers
	for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
	{
		vkDestroyFramebuffer(logicalDevice, swapChainFramebuffers[i], nullptr);
	}

	// Destroys the swap chain
	vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
}

/*********************************************************************************************/
/*!
	\brief
		Cleans up the debug messenger

	\param instance
		The Vulkan instance

	\param debugMessenger,
		The messenger info struct to be cleaned up

	\param pAllocator
		the allocator callback function
*/
/*********************************************************************************************/
void Window::DestroyDebugUtilsMessengerEXT(
	VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator)
{
	// Gets the debug messenger to destroy it
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

/*********************************************************************************************/
/*!
	\brief
		Creates the virtual vulkan logic device
*/
/*********************************************************************************************/
void Window::CreateLogicalDevice()
{
	// Gets the queues that can communicate to the physical card
	QueueFamilyIndices indices = FindQueueFamilies(physicalCard);

	// Creates the structs that hold the info for the virtual command queues
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	// Populates the queue info structs
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		// Creates and initializes a new queue info struct
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// Sets the device features we want
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	// The info struct for the command queues and extensions the device is using
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	// If the validation layers are on
	if (enableValidationLayers)
	{
		// Tell the device the validation layers
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	// Otherwise ignore the validation layers
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	// Creates the logical device
	CheckVulkanSuccess(vkCreateDevice(physicalCard, &createInfo, nullptr, &logicalDevice), "Failed to create logical device queue");
	
	// Sets up the command queues on the new logical device
	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
}

/*********************************************************************************************/
/*!
	\brief
		Sees if the validation layers are on

	\return
		True if validation layers are on, False if not
*/
/*********************************************************************************************/
bool Window::CheckValidationLayerSupport()
{
	// Gets the validation layers
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Checks each validation layer
	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;	// Keeps track of if a validation layer was found

		// Goes through each available layer
		for (const auto& layerProperties : availableLayers)
		{
			// If the layer name matches one of the ones in our list
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				// Go to return true
				layerFound = true;
				break;
			}
		}

		// Otherwise return false
		if (!layerFound)
		{
			return false;
		}
	}

	// Otherwise return true
	return true;
}

/*********************************************************************************************/
/*!
	\brief
		Returns the list of extensions needed for the program

	\return
		The list of extensions for the program
*/
/*********************************************************************************************/
std::vector<const char*> Window::GetRequiredExtensions()
{
	// Fetches the required extensions
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// Combines the extensions to be returned
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	// If validations are on, we also need the validation layers extension
	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	// Returns the extension list
	return extensions;
}

/*********************************************************************************************/
/*!
	\brief
		Creates the swap chain
*/
/*********************************************************************************************/
void Window::CreateSwapChain()
{
	// Gets the swap chain support details
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalCard);

	// Uses those details to find the format, presentation mode, and dimensions of the swap chain
	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);
	
	// Sets the number of images that will be available in the swap chain queue to one over the card's minimum
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	// Ensures the image count isn't past the card's maximum
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	// Creates an info block for the swap chain
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	
	// Gets the graphics and presentations queue
	QueueFamilyIndices indices = FindQueueFamilies(physicalCard);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	// If the graphics and presentation queues are different queues, lets those queues share ownership
	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	// Otherwise keeps drawn frames exclusive to the one shared queue
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	// Sets basic formats for the swap chain rendering
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	// Specified null to assume we only need one swap chain (this means we can't resize the window)
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	// Checks that the swap chain was created succesfully
	if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create the swap chain");
	}

	// Sets the number of frame buffers
	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());
	
	// Sets the image format and dimensions
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

/*********************************************************************************************/
/*!
	\brief
		Defines how to view the images for the screen to read them properly
*/
/*********************************************************************************************/
void Window::CreateImageViews()
{
	// Ties the swap chain to the post process pass because it draws directly to the screen
	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		postProcessPass.AddImageView(CreateImageView(swapChainImages[i], swapChainImageFormat));
	}
	
	// Creates the textures the offscreen render passes are writing to
	baseScenePass.CreateTexture(swapChainExtent.width, swapChainExtent.height, swapChainImageFormat);
	glitchMaskPass.CreateTexture(swapChainExtent.width, swapChainExtent.height, swapChainImageFormat);
}

/*********************************************************************************************/
/*!
	\brief
		Defines the graphics pipeline
*/
/*********************************************************************************************/
void Window::CreateGraphicsPipeline()
{
	// Creates the pipeline cache to track different pipeline stages
	VkPipelineCacheCreateInfo pipelineCacheCreateInfo{};
	pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	if (vkCreatePipelineCache(logicalDevice, &pipelineCacheCreateInfo, NULL, &pipelineCache) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline cache");
	}

	// Sets the push constants for the pipeline
	VkPushConstantRange psRange;
	psRange.offset = 0;
	psRange.size = sizeof(glm::mat4) + sizeof(glm::vec4);
	psRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	// Creates the graphics pipelines
	postProcessPass.CreateGraphicsPipeline(logicalDevice, pipelineCache, "source/Shaders/post_process_vert.spv", "source/Shaders/fisheye_frag.spv", {});
	glitchMaskPass.CreateGraphicsPipeline(logicalDevice, pipelineCache, "source/Shaders/2d_vert.spv", "source/Shaders/2d_frag.spv", { textureDescriptorSetLayout }, &psRange);
	baseScenePass.CreateGraphicsPipeline(logicalDevice, pipelineCache, "source/Shaders/2d_vert.spv", "source/Shaders/2d_frag.spv", { textureDescriptorSetLayout }, &psRange);
}

/*********************************************************************************************/
/*!
	\brief
		Selects a swap chain surface format

	\param availableFormats
		The list of formats to choose from

	\return
		The format we want
*/
/*********************************************************************************************/
VkSurfaceFormatKHR Window::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	// Checks each of the available formats
	for (const auto& availableFormat : availableFormats)
	{
		// We want a nonlinear SRGB format
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	// Otherwise returns the first available format
	return availableFormats[0];
}

/*********************************************************************************************/
/*!
	\brief
		Selects a swap chain presentation format

	\param availablePresentModes
		The list of formats to choose from

	\return
		The format we want
*/
/*********************************************************************************************/
VkPresentModeKHR Window::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	// Goes through each available mode
	for (const auto& availablePresentMode : availablePresentModes)
	{
		// We want the mailbox triple-buffered mode
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

/*********************************************************************************************/
/*!
	\brief
		Selects the resolution of the swap chain

	\param capabilities
		The list of possible resolutions

	\return
		The struct containing the width and height of the swap chain we are drawing to
*/
/*********************************************************************************************/
VkExtent2D Window::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	// If the window is the max size, then we don't need to do anything
	if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)())
	{
		return capabilities.currentExtent;
	}
	// Otherwise calculates the width and height within the margins
	else
	{
		// Gets the framebuffer dimensions and makes an extent
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

		// Clamps the extent to the graphics capabilities
		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		// Returns the calculated extent
		return actualExtent;
	}
}

/*********************************************************************************************/
/*!
	\brief
		Creates the render pass
*/
/*********************************************************************************************/
void Window::CreateRenderPass()
{
	// Creates the render pass
	postProcessPass.CreateRenderPass(true, logicalDevice, swapChainImageFormat);
	glitchMaskPass.CreateRenderPass(false, logicalDevice, swapChainImageFormat);
	baseScenePass.CreateRenderPass(false, logicalDevice, swapChainImageFormat);
}

/*********************************************************************************************/
/*!
	\brief
		Creates the frame buffers for the swap chain
*/
/*********************************************************************************************/
void Window::CreateFramebuffers()
{
	// Creates the framebuffers for the offscreen render passes
	baseScenePass.SetFramebuffers(logicalDevice, swapChainExtent);
	glitchMaskPass.SetFramebuffers(logicalDevice, swapChainExtent);

	// The post process pass is tied to the framebuffers so it gets a separate call
	postProcessPass.SetSwapChainFramebuffers(logicalDevice, swapChainFramebuffers, swapChainExtent);
}

/*********************************************************************************************/
/*!
	\brief
		Creates the command pool that takes in draw commands
*/
/*********************************************************************************************/
void Window::CreateCommandPool()
{
	// Gets the render queues
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalCard);

	// Sets the info for the command pool
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	// Ensures that the command pool was created correctly
	CheckVulkanSuccess(vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool), "failed to create command pool!");

	// Creates the vertex and indices buffers
	CreateVulkanBuffer(vertexBuffer, vertexBufferMemory, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, sizeof(defaultRect[0]) * defaultRect.size(), (void*)defaultRect.data());
	CreateVulkanBuffer(indexBuffer, indexBufferMemory, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, sizeof(defaultRectIndices[0]) * defaultRectIndices.size(), (void*)defaultRectIndices.data());

	// Sets the number of command buffers
	commandBuffer.resize(MAX_FRAMES_IN_FLIGHT);

	// Makes the command buffer info
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffer.size();

	// Checks that the command buffer was created correctly
	CheckVulkanSuccess(vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffer.data()), "failed to allocate command buffers!");
}

/*********************************************************************************************/
/*!
	\brief
		Private function prepare for drawing to the screen

	\param commandBuffer
		The command buffer to record the command to

	\param imageIndex
		The frame buffer index to use the command on
*/
/*********************************************************************************************/
void Window::SetupCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	// Creates the command info
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	// Checks that the command info was initialized correctly
	CheckVulkanSuccess(vkBeginCommandBuffer(commandBuffer, &beginInfo), "failed to begin recording command buffer!");
}

/*********************************************************************************************/
/*!
	\brief
		Private function to record a graphics command

	\param commandBuffer
		The command buffer to record the command to
*/
/*********************************************************************************************/
void Window::RecordCommandBuffer(VkCommandBuffer commandBuffer)
{
	// Sets the vertex buffers
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	// Sets the index buffer
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

	// THE MOTHAFUCKIN' DRAW CALL
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(defaultRectIndices.size()), 1, 0, 0, 0);
}

/*********************************************************************************************/
/*!
	\brief
		Creates the objects to synchronize the swap chain
*/
/*********************************************************************************************/
void Window::CreateSyncObjects()
{
	// Sets the sizes of the sync object vectors
	availableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
	finishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFence.resize(MAX_FRAMES_IN_FLIGHT);

	// Creates the semaphore info
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	// Creates the fence info
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	// Checks that the semaphore and fence were created correctly
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		// Creates the thread syncing objects
		CheckVulkanSuccess(vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &availableSemaphore[i]), "Failed to create synchronization objects for a frame!");
		CheckVulkanSuccess(vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &finishedSemaphore[i]), "Failed to create synchronization objects for a frame!");
		CheckVulkanSuccess(vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFence[i]), "Failed to create synchronization objects for a frame!");
	}
}

/*********************************************************************************************/
/*!
	\brief
		Creates the vertex buffer

	\param buffer
		The buffer to be created

	\param bufferMemory
		The pointer to the buffer

	\param bufferFlags
		The properties of the buffer

	\param bufferSize
		The size of the buffer

	\param rawData
		The data to be written to the new buffer
*/
/*********************************************************************************************/
void Window::CreateVulkanBuffer(VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkBufferUsageFlags bufferFlags, VkDeviceSize bufferSize, void* rawData)
{
	// Creates a staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	// Writes the vertex data into the staging buffer
	void* data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, rawData, (size_t)bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	// Creates the vertex buffer
	CreateBuffer(bufferSize, bufferFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);

	// Copies the staging buffer data to the vertex buffer
	CopyBuffer(stagingBuffer, buffer, bufferSize);

	// Cleans up the staging buffer
	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

/*********************************************************************************************/
/*!
	\brief
		Determines what type of memory the graphics device is using

	\param typeFilter
		Has the memory types we don't want bit-flagged

	\param properties
		Has the properties of our memory needs

	\return
		Success value
*/
/*********************************************************************************************/
uint32_t Window::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	// Gets the memory properties of the graphics device
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalCard, &memProperties);

	// Checks for the memory types the graphics device has
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		// If a memory type matches the type filter, skip it, otherwise check if its a type we want
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	// If everything fails, throw an error that no memory type was found
	throw std::runtime_error("failed to find suitable memory type!");
}

/*********************************************************************************************/
	/*!
		\brief
			Creates a buffer in graphics memory

		\param size
			The size of the buffer

		\param usage
			Flags for what the buffer will be used for

		\param properties
			Flags for the properties of the buffer

		\param buffer
			Return value for the resulting buffer

		\param bufferMemory
			Pointer to the buffer in memory
	*/
	/*********************************************************************************************/
void Window::CreateBuffer(
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkBuffer& buffer,
	VkDeviceMemory& bufferMemory)
{
	// Creates the vertex buffer
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	// Checks that the vertex buffer was created correctly
	CheckVulkanSuccess(vkCreateBuffer(logicalDevice, &bufferInfo, NULL, &buffer), "failed to create buffer!");

	// Gets the memory requirements for what we want to do
	VkMemoryRequirements memRequirements;
 	vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

	// Info block for allocating graphics memory
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	// Checks that memory allocates correctly
	CheckVulkanSuccess(vkAllocateMemory(logicalDevice, &allocInfo, NULL, &bufferMemory), "failed to allocate buffer memory!");

	// Binds the vertex buffer to the allocated memory
	vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
}

/*********************************************************************************************/
/*!
	\brief
		Copies the data from one buffer into another

	\param srcBuffer
		The buffer to be copied from

	\param dstBuffer
		The buffer to be written to

	\param size
		The size of the copy
*/
/*********************************************************************************************/
void Window::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	// Makes that temporary command buffer
	VkCommandBuffer commandBuffer_ = BeginSingleTimeCommands();

	// This is the copy command sent to the command buffer
	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer_, srcBuffer, dstBuffer, 1, &copyRegion);

	// Closes the command buffer
	EndSingleTimeCommands(commandBuffer_);
}

/*********************************************************************************************/
/*!
	\brief
		Sets the descriptor for the uniform buffer object
*/
/*********************************************************************************************/
void Window::CreateDescriptorSetLayout()
{
	// Creates the first layer of the descriptor set layout for the render passes
	std::vector<VkDescriptorType> postProcessDescriptor{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER };
	postProcessPass.CreateDescriptorSetLayout(logicalDevice, VK_SHADER_STAGE_FRAGMENT_BIT, postProcessDescriptor);
	glitchMaskPass.CreateDescriptorSetLayout(logicalDevice, VK_SHADER_STAGE_VERTEX_BIT, { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER });
	baseScenePass.CreateDescriptorSetLayout(logicalDevice, VK_SHADER_STAGE_VERTEX_BIT, { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER });

	// Creates the sampler layout
	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 0;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = NULL;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	
	// Sets the layout for the descriptor set
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &samplerLayoutBinding;
	CheckVulkanSuccess(vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &textureDescriptorSetLayout), "failed to create texture descriptor set layout!");
}

/*********************************************************************************************/
/*!
	\brief
		Creates uniform buffer objects for each frame buffer
*/
/*********************************************************************************************/
void Window::CreateUniformBuffers()
{
	// Sets the size of the buffer
	VkDeviceSize fisheyeUniformBufferSize = sizeof(FisheyeUniformBufferObject);
	VkDeviceSize defaultUniformBufferSize = sizeof(UniformBufferObject);

	// The scene render pass and the glitch mask render pass use a single default uniform buffer
	baseScenePass.CreateUniformBuffers(logicalDevice, defaultUniformBufferSize, 1);
	glitchMaskPass.CreateUniformBuffers(logicalDevice, defaultUniformBufferSize, 1);

	// The onscreen render pass has a fisheye ubo for each swap chain framebuffer
	postProcessPass.CreateUniformBuffers(logicalDevice, fisheyeUniformBufferSize, MAX_FRAMES_IN_FLIGHT);
}

/*********************************************************************************************/
/*!
	\brief
		Updates the uniform buffer objects

	\param currentImage
		The image index for the current frame
*/
/*********************************************************************************************/
void Window::UpdateUniformBuffers(uint32_t currentImage)
{
	// Adjusts the camera movements according to how far along the simulation should be at this time
	UniformBufferObject ubo{};
	ubo.model = camera->GetTranformationMatrix();
	ubo.view = camera->GetViewMatrix();
	ubo.proj = camera->GetPerspectiveMatrix();
	ubo.lookAt = camera->GetLookAtVector();
	ubo.camPos = camera->Get3DPosition();

	// Sets the values for the fisheye effect's uniform buffer
	FisheyeUniformBufferObject fubo{};
	fubo.fisheyeStrength = 0.23f;
	fubo.screenWidth = (float)swapChainExtent.width;
	fubo.screenHeight = (float)swapChainExtent.height;

	// Sets the uniform buffers
	baseScenePass.UpdateUniformBuffer(logicalDevice, sizeof(ubo), &ubo);
	glitchMaskPass.UpdateUniformBuffer(logicalDevice, sizeof(ubo), &ubo);
	postProcessPass.UpdateUniformBuffer(logicalDevice, sizeof(fubo), &fubo, currentFrame);
}

/*********************************************************************************************/
/*!
	\brief
		Creates a pool of info for the shader descriptors
*/
/*********************************************************************************************/
void Window::CreateDescriptorPool()
{
	// Defines the number of descriptors in the pools
	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) + 8;
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = 64;

	// Sets the pool info based on the pool size
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) + 64 + 8;

	// Creates the descriptor pool
	CheckVulkanSuccess(vkCreateDescriptorPool(logicalDevice, &poolInfo, NULL, &descriptorPool), "failed to create descriptor pool!");
}

/*********************************************************************************************/
/*!
	\brief
		Allocates descriptor sets
*/
/*********************************************************************************************/
void Window::CreateDescriptorSets()
{
	// Creates the descriptor sets for the render passes
	baseScenePass.CreateDescriptorSet(logicalDevice, descriptorPool, 1);
	glitchMaskPass.CreateDescriptorSet(logicalDevice, descriptorPool, 1);
	postProcessPass.CreateDescriptorSet(logicalDevice, descriptorPool, MAX_FRAMES_IN_FLIGHT);
	
	// Updates the newly created descriptor sets
	UpdateDescriptorSets();
}

/*********************************************************************************************/
/*!
	\brief
		Updates the descriptor sets
*/
/*********************************************************************************************/
void Window::UpdateDescriptorSets()
{
	// Updates the uniform buffers of the descriptor sets
	baseScenePass.UpdateDescriptorSetUniformBuffer(logicalDevice, sizeof(UniformBufferObject));
	glitchMaskPass.UpdateDescriptorSetUniformBuffer(logicalDevice, sizeof(UniformBufferObject));
	postProcessPass.UpdateDescriptorSetUniformBuffer(logicalDevice, sizeof(FisheyeUniformBufferObject));

	// The post process pass has other descriptor sets besides the uniform buffer that need to be manually set
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		// The additional descriptor sets use the combined image sampler type so they can be read as textures
		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = postProcessPass.GetDescriptorSets()[i];
		descriptorWrite.dstBinding = 1;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;

		// The first additional descriptor is the base scene pass's output
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = baseScenePass.GetOutputTexture()->GetImageView();
		imageInfo.sampler = baseScenePass.GetOutputTexture()->GetSampler();
		descriptorWrite.pImageInfo = &imageInfo;
		vkUpdateDescriptorSets(logicalDevice, 1, &descriptorWrite, 0, NULL);

		// The second descriptor is the glitch mask output
		imageInfo.imageView = glitchMaskPass.GetOutputTexture()->GetImageView();
		imageInfo.sampler = glitchMaskPass.GetOutputTexture()->GetSampler();
		descriptorWrite.dstBinding = 2;
		vkUpdateDescriptorSets(logicalDevice, 1, &descriptorWrite, 0, NULL);
	}
}

/*********************************************************************************************/
/*!
	\brief
		Converts the given buffer data into a Vulkan image

	\param image
		The image object that the view is being created for

	\param format
		How to format the image view object

	\return
		The resultant image view object
*/
/*********************************************************************************************/
VkImageView Window::CreateImageView(VkImage image, VkFormat format)
{
	VkImageViewCreateInfo createInfo{};		// Holds the image information

	// Defines that we are making an image view information block
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;

	// We're making a 2d image
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = format;

	// We want to change colors around
	//createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	//createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	//createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	//createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	// We have a basic, 1 layer image
	createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	// Makes the image view
	VkImageView imageView;
	if (vkCreateImageView(logicalDevice, &createInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to make the image view");
	}

	// returns the newly made image view object
	return imageView;
}

/*********************************************************************************************/
/*!
	\brief
		Runs the render pass for the post process shaders
*/
/*********************************************************************************************/
void Window::RunFisheyeRenderPass()
{
	// Sets the info for the post-processing render pass
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = postProcessPass.GetRenderPass();
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;
	VkClearValue clearColor = { {{0.0f, 0.1f, 0.2f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	
	// Starts the render pass
	vkCmdBeginRenderPass(commandBuffer[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Sets the fisheye lense pass
	vkCmdBindDescriptorSets(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, postProcessPass.GetGraphicsPipelineLayout(), 0, 1, &postProcessPass.GetDescriptorSets()[currentFrame], 0, NULL);
	vkCmdBindPipeline(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, postProcessPass.GetGraphicsPipeline());
	vkCmdDraw(commandBuffer[currentFrame], 3, 1, 0, 0);

	// Ends the render pass
	vkCmdEndRenderPass(commandBuffer[currentFrame]);
}
