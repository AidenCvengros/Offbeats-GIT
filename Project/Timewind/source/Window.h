/*************************************************************************************************/
/*!
\file Window.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2023.9.7
\brief
	Opens and manages the program's OpenGL window.

	Public Functions:
		+ Window::Window
		+ Window::~Window
		+ Window::Init
		+ Window::Update
		+ Window::Draw
		+ Window::Shutdown

	Private Functions:

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_Window_H_
#define Syncopatience_Window_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"

#include "System.h"
#include <optional>
#include <vector>

// Includes glfw library for window management
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

// glm matrix and math functionality
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// The camera class for displaying to the screen
#include "Camera.h"

// The texture class for the default texture
#include "Texture.h"

// The render pass class
#include "RenderPass.h"

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
		Creates and manages the Vulkan window
*/
/*************************************************************************************************/
class Window : System
{
private:
	GLFWwindow* window;									// Pointer to the window. NEEDS TO BE THE FIRST VARIABLE

public:
	//---------------------------------------------------------------------------------------------
	// Public Consts
	//---------------------------------------------------------------------------------------------
	
	const int MAX_FRAMES_IN_FLIGHT = 2;

	//---------------------------------------------------------------------------------------------
	// Public Structures
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Variables
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Public Function Declarations
	//---------------------------------------------------------------------------------------------

	/*********************************************************************************************/
	/*!
		\brief
			Constructor for the window class

		\param initWidth
			The width of the constructed window

		\param initHeight
			The height of the constructed window

		\param initWindowName
			The string for the window title
	*/
	/*********************************************************************************************/
	Window(int initWidth, int initHeight, std::string initWindowName) :
		window(NULL), vulkanInstance(NULL), physicalCard(NULL), logicalDevice(NULL),
		graphicsQueue(NULL), surface(NULL), presentQueue(NULL), commandPool(NULL), commandBuffer(),
		imageIndex(0), textureDescriptorSetLayout(NULL), descriptorPool(NULL), pipelineCache(NULL), debugMessenger(NULL),
		swapChain(NULL), swapChainImages(0), swapChainImageFormat(), swapChainExtent(), swapChainFramebuffers(),
		camera(NULL), blankTexture(NULL),
		baseScenePass(), glitchMaskPass(), postProcessPass(),
		vertexBuffer(NULL), vertexBufferMemory(NULL), indexBuffer(NULL), indexBufferMemory(NULL),
		availableSemaphore(), finishedSemaphore(), inFlightFence(),
		currentFrame(0), framebufferResized(false), width(initWidth), height(initHeight), name(initWindowName) 
	{
	}

	/*********************************************************************************************/
	/*!
		\brief
			Deconstructor for the window
	*/
	/*********************************************************************************************/
	~Window();

	/*********************************************************************************************/
	/*!
		\brief
			Initializes for the window
	*/
	/*********************************************************************************************/
	void Init();

	/*********************************************************************************************/
	/*!
		\brief
			Update function for the window

		\param dt
			The time elapsed since the previous frame
	*/
	/*********************************************************************************************/
	void Update(double dt);

	/*********************************************************************************************/
	/*!
		\brief
			Begins the drawing sequence

		\param
			The window being drawn to
	*/
	/*************************************************************************************************/
	void Draw(Window* window);

	/*********************************************************************************************/
	/*!
		\brief
			Begins the mask render pass drawing sequence

		\param
			The window being drawn to
	*/
	/*************************************************************************************************/
	void DrawMaskRenderPass(Window* window);

	/*********************************************************************************************/
	/*!
		\brief
			Draws the given game object

		\param gameObject
			The given object to be drawn
	*/
	/*********************************************************************************************/
	void DrawGameObject(GameObject* gameObject);

	/*********************************************************************************************/
	/*!
		\brief
			Ends the drawing sequence
	*/
	/*********************************************************************************************/
	void CleanupDraw();

	/*********************************************************************************************/
	/*!
		\brief
			Shuts down the window
	*/
	/*********************************************************************************************/
	void Shutdown();

	/*********************************************************************************************/
	/*!
		\brief
			Gets the pointer to the window

		\return
			The window pointer
	*/
	/*********************************************************************************************/
	GLFWwindow* GetWindowPtr() { return window; }

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
			Vulkan api error message
	*/
	/*********************************************************************************************/
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	/*********************************************************************************************/
	/*!
		\brief
			The callback function for when the window is resized

		\param window
			Pointer to the window

		\param width
			The new width of the window

		\param height
			The new height of the window
	*/
	/*********************************************************************************************/
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

	/*********************************************************************************************/
	/*!
		\brief
			Returns the dimensions of the window

		\return
			The window's dimensions
	*/
	/*********************************************************************************************/
	glm::vec2 GetWindowSize() { return glm::vec2(swapChainExtent.width, swapChainExtent.height); }

	/*********************************************************************************************/
	/*!
		\brief
			The callback function for when the window is resized

		\param newCamera
			The new camera
	*/
	/*********************************************************************************************/
	void SetCamera(Camera* newCamera) { camera = newCamera; }

	/*********************************************************************************************/
	/*!
		\brief
			Returns the vulkan reference of the graphics card

		\return
			The graphics card
	*/
	/*********************************************************************************************/
	VkPhysicalDevice GetPhysicalDevice() { return physicalCard; }

	/*********************************************************************************************/
	/*!
		\brief
			Returns the vulkan device to the graphics card

		\return
			The vulkan device
	*/
	/*********************************************************************************************/
	VkDevice& GetLogicalDevice() { return logicalDevice; }

	/*********************************************************************************************/
	/*!
		\brief
			Returns the descriptor pool

		\return
			The descriptor pool for this vulkan instance
	*/
	/*********************************************************************************************/
	VkDescriptorPool GetDescriptorPool() { return descriptorPool; }

	/*********************************************************************************************/
	/*!
		\brief
			Returns the template structure for a texture's descriptor set

		\return
			The descriptor set layout for textures
	*/
	/*********************************************************************************************/
	VkDescriptorSetLayout* GetTextureDescriptorSetLayout() { return &textureDescriptorSetLayout; }

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
	void CreateBuffer(
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);

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
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	/*********************************************************************************************/
	/*!
		\brief
			Starts a command buffer

		\return
			The new command buffer
	*/
	/*********************************************************************************************/
	VkCommandBuffer BeginSingleTimeCommands() const;

	/*********************************************************************************************/
	/*!
		\brief
			Ends the given command buffer

		\param commandBuffer_
			The command buffer to be ended
	*/
	/*********************************************************************************************/
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer_) const;

	/*********************************************************************************************/
	/*!
		\brief
			Waits for all draw commands to finish
	*/
	/*********************************************************************************************/
	void WaitForDrawFinished();

	/*********************************************************************************************/
	/*!
		\brief
			Helper function to create an image view object

		\param image
			The image object that the view is being created for

		\param format
			How to format the image view object

		\return
			The resultant image view object
	*/
	/*********************************************************************************************/
	VkImageView CreateImageView(VkImage image, VkFormat format);

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
	VkResult CheckVulkanSuccess(VkResult functionResult, std::string errorMessage);
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------
	
	// The list of names for valid validation layers for our program
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	// Turns on validation layers when running in debug, and off otherwise
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	//---------------------------------------------------------------------------------------------
	// Private Structures
	//---------------------------------------------------------------------------------------------

	/*********************************************************************************************/
	/*!
		\brief
			Tracks the graphics card's ability to support our graphics queue needs
	*/
	/*********************************************************************************************/
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;	// The queue type for graphics queues
		std::optional<uint32_t> presentFamily;	// The queue type for presentation queues

		/*****************************************************************************************/
		/*!
			\brief
				Checks that the queue family has a complete set of indices
		
			\result
				Whether the struct is complete
		*/
		/*****************************************************************************************/
		bool IsComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

	/*********************************************************************************************/
	/*!
		\brief
			Handles variables needed to create the swap chain for this device
	*/
	/*********************************************************************************************/
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities = { 0 };		// Holds the capabilities of the screen surface
		std::vector<VkSurfaceFormatKHR> formats;	// Tracks how the screen wants info formatted
		std::vector<VkPresentModeKHR> presentModes;	// What presentation modes are available
	};

	/*********************************************************************************************/
	/*!
		\brief
			The uniform buffer object for the first vertex shader
	*/
	/*********************************************************************************************/
	struct UniformBufferObject
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
		glm::vec4 lookAt;
		glm::vec4 camPos;
		//glm::vec4 playerPos;
	};

	/*********************************************************************************************/
	/*!
		\brief
			The uniform buffer object for the post processing shader
	*/
	/*********************************************************************************************/
	struct FisheyeUniformBufferObject
	{
		float fisheyeStrength;
		float screenWidth;
		float screenHeight;
	};
	
	//---------------------------------------------------------------------------------------------
	// Private Variables
	//---------------------------------------------------------------------------------------------
	
	// Important objects
	VkInstance vulkanInstance;							// Handles the vulkan instance
	VkPhysicalDevice physicalCard;						// Tracks the graphics card being used
	VkDevice logicalDevice;								// Holds the logical device that communicates with the graphics card

	// Graphics objects
	VkQueue graphicsQueue;								// Holds the command queue for the logical device
	VkSurfaceKHR surface;								// The virtual screen surface to be drawn to
	VkQueue presentQueue;								// Holds the command queue for drawing to the surface
	VkCommandPool commandPool;							// Holds the list of commands for the graphics system
	std::vector<VkCommandBuffer> commandBuffer;			// Gets the commands
	uint32_t imageIndex;								// Keeps track of which image the swap chain is on
	VkDescriptorSetLayout textureDescriptorSetLayout;	// Specifies the layout for texture descriptors.
	VkDescriptorPool descriptorPool;					// The descriptor pool
	VkPipelineCache pipelineCache;						// The cache to track different stages of the graphics pipeline
	VkDebugUtilsMessengerEXT debugMessenger;			// The debug messenger

	// Swap Chain objects
	VkSwapchainKHR swapChain;							// The swap chain
	std::vector<VkImage> swapChainImages;				// The frame buffers being drawn to
	VkFormat swapChainImageFormat;						// The image format (should be srgb)
	VkExtent2D swapChainExtent;							// Holds the dimensions of the frame buffers
	std::vector<VkFramebuffer> swapChainFramebuffers;	// The framebuffers for the swap chain
	
	// Game Dependencies
	Camera* camera;										// The main camera object
	Texture* blankTexture;								// Default texture used to draw game objects without a texture

	// Render Passes
	RenderPass baseScenePass;							// The render pass for drawing the base scene
	RenderPass glitchMaskPass;							// The render pass that creates the mask for where to render glitch effects
	RenderPass postProcessPass;							// The post-processing render pass

	// Memory Buffers
	VkBuffer vertexBuffer;								// The vertex buffer
	VkDeviceMemory vertexBufferMemory;					// The memory pointer for the vertex buffer
	VkBuffer indexBuffer;								// The indices corresponding to vertices in the vertex buffer
	VkDeviceMemory indexBufferMemory;					// The memory pointer for the index buffer

	// Thread Syncing
	std::vector<VkSemaphore> availableSemaphore;		// The semaphore for checking if the buffer is available
	std::vector<VkSemaphore> finishedSemaphore;			// The semaphore for when the render is finished
	std::vector<VkFence> inFlightFence;					// The fence to make sure only one frame happens at a time

	// Window variables
	uint32_t currentFrame = 0;							// Keeps track of the frame count
	bool framebufferResized = false;					// Manually tells the program to recreate the swap chain
	int width;											// The width of the window
	int height;											// The height of the window
	std::string name;									// The name of the window

	// Holds the list of graphics device extensions that we want
	const std::vector<const char*> deviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------

	//// Initialization Functions

	/*********************************************************************************************/
	/*!
		\brief
			Initializes Vulkan
	*/
	/*********************************************************************************************/
	void CreateVulkanInstance();

	/*********************************************************************************************/
	/*!
		\brief
			Sets up the debug messages
	*/
	/*********************************************************************************************/
	void SetupDebugMessenger();

	/*********************************************************************************************/
	/*!
		\brief
			Initializes the virtual screen surface
	*/
	/*********************************************************************************************/
	void InitializeSurface();

	//// Helper Functions

	/*********************************************************************************************/
	/*!
		\brief
			Makes the info struct for debug messages

		\param createInfo
			The output info struct
	*/
	/*********************************************************************************************/
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

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
	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);

	//// Destruction Functions

	/*********************************************************************************************/
	/*!
		\brief
			Remakes the swap chain if it needs changes
	*/
	/*********************************************************************************************/
	void RecreateSwapChain();

	/*********************************************************************************************/
	/*!
		\brief
			Destroys the swap chain
	*/
	/*********************************************************************************************/
	void CleanupSwapChain();

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
	void DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);

	/*********************************************************************************************/
	/*!
		\brief
			Picks a graphics (or other valid) card for Vulkan to render on
	*/
	/*********************************************************************************************/
	void PickPhysicalDevice();

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
	bool IsDeviceSuitable(VkPhysicalDevice device);

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
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	/*********************************************************************************************/
	/*!
		\brief
			Creates the virtual vulkan logic device
	*/
	/*********************************************************************************************/
	void CreateLogicalDevice();

	/*********************************************************************************************/
	/*!
		\brief
			Sees if the validation layers are on

		\return
			True if validation layers are on, False if not
	*/
	/*********************************************************************************************/
	bool CheckValidationLayerSupport();

	/*********************************************************************************************/
	/*!
		\brief
			Returns the list of extensions needed for the program

		\return
			The list of extensions for the program
	*/
	/*********************************************************************************************/
	std::vector<const char*> GetRequiredExtensions();

	/*********************************************************************************************/
	/*!
		\brief
			Creates the swap chain
	*/
	/*********************************************************************************************/
	void CreateSwapChain();

	/*********************************************************************************************/
	/*!
		\brief
			Defines how to view the images for the screen to read them properly
	*/
	/*********************************************************************************************/
	void CreateImageViews();

	/*********************************************************************************************/
	/*!
		\brief
			Defines the graphics pipeline
	*/
	/*********************************************************************************************/
	void CreateGraphicsPipeline();

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
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

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
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

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
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

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
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

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
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	/*********************************************************************************************/
	/*!
		\brief
			Creates the render pass
	*/
	/*********************************************************************************************/
	void CreateRenderPass();

	/*********************************************************************************************/
	/*!
		\brief
			Creates the frame buffers for the swap chain
	*/
	/*********************************************************************************************/
	void CreateFramebuffers();

	/*********************************************************************************************/
	/*!
		\brief
			Creates the command pool that takes in draw commands
	*/
	/*********************************************************************************************/
	void CreateCommandPool();

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
	void SetupCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	/*********************************************************************************************/
	/*!
		\brief
			Private function to record a graphics command

		\param commandBuffer
			The command buffer to record the command to
	*/
	/*********************************************************************************************/
	void RecordCommandBuffer(VkCommandBuffer commandBuffer);

	/*********************************************************************************************/
	/*!
		\brief
			Creates the objects to synchronize the swap chain
	*/
	/*********************************************************************************************/
	void CreateSyncObjects();

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
	void CreateVulkanBuffer(VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkBufferUsageFlags bufferFlags, VkDeviceSize bufferSize, void* rawData);

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
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	/*********************************************************************************************/
	/*!
		\brief
			Sets the descriptor for the uniform buffer object
	*/
	/*********************************************************************************************/
	void CreateDescriptorSetLayout();

	/*********************************************************************************************/
	/*!
		\brief
			Creates uniform buffer objects
	*/
	/*********************************************************************************************/
	void CreateUniformBuffers();

	/*********************************************************************************************/
	/*!
		\brief
			Updates the uniform buffer objects

		\param currentImage
			The image index for the current frame
	*/
	/*********************************************************************************************/
	void UpdateUniformBuffers(uint32_t currentImage);

	/*********************************************************************************************/
	/*!
		\brief
			Creates a pool of info for the shader descriptors
	*/
	/*********************************************************************************************/
	void CreateDescriptorPool();

	/*********************************************************************************************/
	/*!
		\brief
			Allocates descriptor sets
	*/
	/*********************************************************************************************/
	void CreateDescriptorSets();

	/*********************************************************************************************/
	/*!
		\brief
			Updates the descriptor sets
	*/
	/*********************************************************************************************/
	void UpdateDescriptorSets();

	/*********************************************************************************************/
	/*!
		\brief
			Runs the render pass for the post process shaders
	*/
	/*********************************************************************************************/
	void RunFisheyeRenderPass();
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_Window_H_
