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

#include "Window.h"

// Includes set for making lists
#include <set>

// Includes for mathing
#include <cstdint>
#include <limits>
#include <algorithm>

// Shader file loading
#include <fstream>

// Functionality for running and drawing to the window
#include "Vertex.h"
#include <chrono>

// Game Object object to keep track of things to draw to the screen
#include "GameObject.h"

//-------------------------------------------------------------------------------------------------
// Private Constants
//-------------------------------------------------------------------------------------------------

// The default triangle																				CURRENTLY COMMENTED OUT UNTIL WE NEED IT
//static const std::vector<Vertex> defaultTri =
//{
//	{{0.0f, -0.5f}, {1.0f, 0.2f, 0.2f}, {1.0f, 0.0f}},
//	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.5f}, {0.5f, 1.0f}},
//	{{-0.5f, 0.5f}, {0.0f, 0.5f, 1.0f}, {1.0f, 1.0f}}
//};

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
		Initializes for the window
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
	InitSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	createSwapChain();
	createImageViews();
	CreateRenderPass();
	CreateDescriptorSetLayout();
	createGraphicsPipeline();
	CreateFramebuffers();
	CreateCommandPool();
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();
	CreateSyncObjects();
	blankTexture = new Texture(this, "Assets/Sprites/Blank.png");
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
	glfwPollEvents();
}

/*********************************************************************************************/
/*!
	\brief
		Draws the window

	\param window_
		The window being drawn to
*/
/*********************************************************************************************/
void Window::Draw(Window* window_)
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
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	// Sets the render size
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	// Start of the render pass
	vkCmdBeginRenderPass(commandBuffer[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Binds the render pass
	vkCmdBindPipeline(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

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
	vkCmdBindDescriptorSets(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, NULL);
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
	if (gameObject->GetRender())
	{
		// Sets the dynamic offset and draws the first object
		glm::mat4 tempMat = gameObject->GetTranformationMatrix();
		glm::vec4 tempColor = gameObject->GetColor();
		vkCmdPushConstants(commandBuffer[currentFrame], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &tempMat);
		vkCmdPushConstants(commandBuffer[currentFrame], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 64, sizeof(glm::vec4), &tempColor);

		// Checks if the game object has a texture
		if (gameObject->GetTexture())
		{
			// Binds the texture descriptor set and color
			vkCmdBindDescriptorSets(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 1, 1, gameObject->GetTexture()->GetDescriptorSet(), 0, NULL);
		}
		// Otherwise uses the default blank texture
		else
		{
			vkCmdBindDescriptorSets(commandBuffer[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 1, 1, blankTexture->GetDescriptorSet(), 0, NULL);
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

	// Checks that everything happened correctly
	if (vkEndCommandBuffer(commandBuffer[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}

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
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

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

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
	{
		framebufferResized = false;
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

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
	blankTexture->Free(this);
	delete blankTexture;

	// Cleans up the swap chain
	CleanupSwapChain();

	// Cleans up the render pass, pipeline layout, and pipeline variables
	vkDestroyPipeline(logicalDevice, graphicsPipeline, NULL);
	vkDestroyPipelineLayout(logicalDevice, pipelineLayout, NULL);
	vkDestroyRenderPass(logicalDevice, renderPass, NULL);

	// Cleans up the uniform buffer objects
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroyBuffer(logicalDevice, uniformBuffers[i], NULL);
		vkFreeMemory(logicalDevice, uniformBuffersMemory[i], NULL);
	}

	// Destroys the descriptor sets
	vkDestroyDescriptorPool(logicalDevice, descriptorPool, NULL);
	vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, NULL);
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
	Window* app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window_));
	app->framebufferResized = true;
}

/*********************************************************************************************/
/*!
	\brief
		Returns the dimensions of the window

	\return
		The window's dimensions
*/
/*********************************************************************************************/
glm::vec2 Window::GetWindowSize()
{
	return glm::vec2(swapChainExtent.width, swapChainExtent.height);
}

/*********************************************************************************************/
/*!
	\brief
		The callback function for when the window is resized

	\param newCamera
		The new camera
*/
/*********************************************************************************************/
void Window::SetCamera(Camera* newCamera)
{
	camera = newCamera;
}

/*********************************************************************************************/
/*!
	\brief
		Starts a command buffer

	\return
		The new command buffer
*/
/*********************************************************************************************/
VkCommandBuffer Window::BeginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer_;
	vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer_);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

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
void Window::EndSingleTimeCommands(VkCommandBuffer commandBuffer_)
{
	vkEndCommandBuffer(commandBuffer_);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer_;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

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

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*********************************************************************************************/
/*!
	\brief
		Initializes Vulkan
*/
/*********************************************************************************************/
void Window::CreateVulkanInstance()
{
	// Checks if validation layers are on
	if(enableValidationLayers && !CheckValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Syncopatience";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Custom Engine made by Aiden Cvengros";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance!");
	}
}

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
	createSwapChain();
	createImageViews();
	CreateFramebuffers();
}

/*********************************************************************************************/
/*!
	\brief
		Destroys the swap chain
*/
/*********************************************************************************************/
void Window::CleanupSwapChain()
{
	// Destroys all the frame buffers
	for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
	{
		vkDestroyFramebuffer(logicalDevice, swapChainFramebuffers[i], nullptr);
	}

	// Destroys all the image views
	for (size_t i = 0; i < imageViews.size(); i++)
	{
		vkDestroyImageView(logicalDevice, imageViews[i], nullptr);
	}

	// Destroys the swap chain
	vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
}

/*********************************************************************************************/
/*!
	\brief
		Sets up the debug messages
*/
/*********************************************************************************************/
void Window::SetupDebugMessenger()
{
	// If we aren't doing validation layers, skip this function
	if (!enableValidationLayers) return;

	// Makes an info stucture for the message
	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	// Makes the debug messenger
	if (CreateDebugUtilsMessengerEXT(vulkanInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

/*********************************************************************************************/
/*!
	\brief
		Makes the info struct for debug messages

	\param createInfo
		The output info struct
*/
/*********************************************************************************************/
void Window::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
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
		Makes sure that the debug utility message system is loaded in

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
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
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
	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) {
			physicalCard = device;
			break;
		}
	}

	if (physicalCard == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find physical graphics device");
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
	bool extensionsSupported = checkDeviceExtensionSupport(device_);

	// Checks if swap chain is suitable for the game
	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device_);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	// Gets the physical device's features
	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device_, &supportedFeatures);

	// If all of that is, returns true
	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

Window::QueueFamilyIndices Window::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		// Gets whether the device can render a surface
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		// If the device can render a surface, save it
		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

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

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalCard, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device queue");
	}
	
	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
}

/*********************************************************************************************/
/*!
	\brief
		Initializes the virtual screen surface
*/
/*********************************************************************************************/
void Window::InitSurface()
{
	if (glfwCreateWindowSurface(vulkanInstance, window, NULL, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to initialize virtual screen surface");
	}
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
		if (!layerFound) {
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
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

/*********************************************************************************************/
/*!
	\brief
		Creates the swap chain
*/
/*********************************************************************************************/
void Window::createSwapChain()
{
	// Gets the swap chain support details
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalCard);

	// Uses those details to find the format, presentation mode, and dimensions of the swap chain
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
	
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
void Window::createImageViews()
{
	// Sets the size of the image views vector
	imageViews.resize(swapChainImages.size());

	// Loops through each image on the swap chain
	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		imageViews[i] = CreateImageView(swapChainImages[i], swapChainImageFormat);
	}
}

/*********************************************************************************************/
/*!
	\brief
		Defines the graphics pipeline
*/
/*********************************************************************************************/
void Window::createGraphicsPipeline()
{
	// Reads in the shaders
	auto vertShaderCode = ReadFile("source/2d_vert.spv");
	auto fragShaderCode = ReadFile("source/2d_frag.spv");

	// Converts the raw data into the shader modules
	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

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

	// Tells it to not have static vertex info
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	// Gets the vertex description structs
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	// Sets the vertex description structs
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

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

	// Sets the push constants for the pipeline
	VkPushConstantRange psRange;
	psRange.offset = 0;
	psRange.size = sizeof(glm::mat4) + sizeof(glm::vec4);
	psRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	// Initializes the pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 2;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &psRange;

	// Checks that the pipeline layout was set up correctly
	if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}

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

	// Checks that the graphics pipeline was created correctly
	if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &graphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	// Cleans up the shader modules
	vkDestroyShaderModule(logicalDevice, fragShaderModule, NULL);
	vkDestroyShaderModule(logicalDevice, vertShaderModule, NULL);
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
bool Window::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	// Gets the total number of available extensions
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	
	// Gets the extensions
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	// Gets a list of the different extensions we need
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	// Checks if all of those extensions are present
	for (const auto& extension : availableExtensions) {
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
Window::SwapChainSupportDetails Window::querySwapChainSupport(VkPhysicalDevice device)
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
VkSurfaceFormatKHR Window::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
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
VkPresentModeKHR Window::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
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
VkExtent2D Window::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	// If the window is the max size, then we don't need to do anything
	if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)())
	{
		return capabilities.currentExtent;
	}
	// Otherwise calculates the width and height within the margins
	else
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

/*********************************************************************************************/
/*!
	\brief
		Reads in the given file

	\param filename
		The given file

	\return
		A string of the file
*/
/*********************************************************************************************/
std::vector<char> Window::ReadFile(const std::string& filename)
{
	// Opens the file
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	// If the file opened correctly
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}

	// Gets the file size
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	// Reads the file from the start
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	// Closes the file
	file.close();

	return buffer;
}

/*********************************************************************************************/
/*!
	\brief
		Formats shaders for use

	\param filename
		The raw shader code

	\return
		The resultant shader module
*/
/*********************************************************************************************/
VkShaderModule Window::createShaderModule(const std::vector<char>& code)
{
	// Converts the raw read in data into a the shader module info struct
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	// The shader module
	VkShaderModule shaderModule;
	
	// Creates and checks that the shader module was created correctly
	if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module!");
	}

	// Returns the shader module
	return shaderModule;
}

/*********************************************************************************************/
/*!
	\brief
		Creates the render pass
*/
/*********************************************************************************************/
void Window::CreateRenderPass()
{
	// Sets up a single color buffer for the swap chain
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

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
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	// Sets up the render pass
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	// Checks that the render pass was created correctly
	if (vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render pass!");
	}
}

/*********************************************************************************************/
/*!
	\brief
		Creates the frame buffers for the swap chain
*/
/*********************************************************************************************/
void Window::CreateFramebuffers()
{
	// Sets the number of frame buffers
	swapChainFramebuffers.resize(imageViews.size());

	// Makes a frame buffer for each slot in the vector
	for (size_t i = 0; i < imageViews.size(); i++)
	{
		VkImageView attachments[] =
		{
			imageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
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
	if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}

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
	if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffer.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
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
	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}
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
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &availableSemaphore[i]) != VK_SUCCESS ||
			vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &finishedSemaphore[i]) != VK_SUCCESS ||
			vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFence[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
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
	if (vkCreateBuffer(logicalDevice, &bufferInfo, NULL, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer!");
	}

	// Gets the memory requirements for what we want to do
	VkMemoryRequirements memRequirements;
 	vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

	// Info block for allocating graphics memory
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	// Checks that memory allocates correctly
	if (vkAllocateMemory(logicalDevice, &allocInfo, NULL, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory!");
	}

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
	// Sets the layout for the uniform buffer object
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.pImmutableSamplers = NULL;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

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
	layoutInfo.pBindings = &uboLayoutBinding;

	// Makes the descriptor set
	if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, NULL, &descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}

	layoutInfo.pBindings = &samplerLayoutBinding;
	
	if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &textureDescriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture descriptor set layout!");
	}
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
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	// Sets there to be a buffer for each framebuffer
	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	// Creates the uniform buffer objects
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

		//vkMapMemory(logicalDevice, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
	}
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

	// Copies all the uniform buffer data in
	void* data;
	vkMapMemory(logicalDevice, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(logicalDevice, uniformBuffersMemory[currentImage]);
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
	poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = 3;

	// Sets the pool info based on the pool size
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) + 3;

	// Creates the descriptor pool
	if (vkCreateDescriptorPool(logicalDevice, &poolInfo, NULL, &descriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

/*********************************************************************************************/
/*!
	\brief
		Allocates descriptor sets
*/
/*********************************************************************************************/
void Window::CreateDescriptorSets()
{
	// Sets the info struct for the descriptor set
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	// Allocates the memory for the descriptor sets
	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	// Initializes the data for each descriptor set
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		// Specifies the buffer size
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

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
