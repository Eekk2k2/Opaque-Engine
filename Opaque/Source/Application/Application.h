#pragma once

#include <iostream>
#include <optional>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <set>
#include <fstream>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Debug/Debug.h"
#include "..\Shaders\ShaderManager.h"

using namespace ShaderManager;

class Application
{
public:

	// ------------------------------ MOVE TO CAMERA ONCE IMPLEMENTED --- TODO ::

	/// <summary>
	/// The x size of the applcation window (*ApplcationWindow from Applcation.h) This value is a constant.
	/// </summary>
	const uint32_t ApplicationWidth = 1920;

	/// <summary>
	/// The y size of the applcation window (*ApplcationWindow from Applcation.h) This value is a constant.
	/// </summary>
	const uint32_t ApplicationHeight = 1080;

	// ------------------------------ Application ---

	/// <summary>
	/// Each time the application name is needed this value will be referenced. This value is a constant.
	/// </summary>
	const char* ApplicationName = "Unnamed Application";

	/// <summary>
	/// The GLFW window object of this application.
	/// </summary>
	GLFWwindow* ApplicationWindow;

	/// <summary>
	/// The GLFW cursor object for this application. Use Application::SetCursor() to change it.
	/// </summary>
	GLFWcursor* ApplicationCursor;

	/// <summary>
	/// This is the debug client of thi application. 
	/// </summary>
	Debug debug = Debug();

	/// <summary>
	/// Function to get the whole thing going. Call this in main.cpp to start the program.
	/// </summary>
	void Run();

private:

	// ------------------------------ Application ---

	/// <summary>
	/// The vulkan instance of this application. Created in Application::CreateVulkanInstance();
	/// </summary>
	VkInstance VulkanInstance;

	/// <summary>
	/// Vulkan surface
	/// </summary>
	VkSurfaceKHR VulkanSurface;

	/// <summary>
	/// This value represents the selected physical device in the Vulkan graphics API 
	/// </summary>
	VkPhysicalDevice VulkanPhysicalDevice = VK_NULL_HANDLE;

	/// <summary>
	/// The logical device of this application. Created in Application::VulkanCreateLogicalDevice();
	/// </summary>
	VkDevice VulkanDevice;

	/// <summary>
	/// Vulkan graphics queue
	/// </summary>
	VkQueue GraphicsQueue;

	/// <summary>
	/// Vulkan present queue
	/// </summary>
	VkQueue PresentQueue;

#ifdef NDEBUG
	/// <summary>
	/// Wether to enable validation layers or not. This value automatically changes based on build type(DEBUG and RELEASE)
	/// </summary>
	const bool EnableValidationLayers = false;
#else
	/// <summary>
	/// Wether to enable validation layers or not. This value automatically changes based on build type(DEBUG and RELEASE)
	/// </summary>
	const bool EnableValidationLayers = true;
#endif

	/// <summary>
	/// Vector over the validation layers for the vulkan instance
	/// </summary>
	/// <param name="LogMessage"></param>
	/// <param name="TYPE"></param>
	const std::vector<const char*> ValidationLayers =
	{
		"VK_LAYER_KHRONOS_validation"
	};

	/// <summary>
	/// I know what this is but not how to explain it TODO : Find out eventually
	/// </summary>
	struct VulkanQueueFamilyIndices
	{
		std::optional<uint32_t> GraphicsFamily;
		std::optional<uint32_t> PresentFamily;

		bool isComplete()
		{
			return GraphicsFamily.has_value() && PresentFamily.has_value();
		}
	};

	/// <summary>
	/// Checking for Swap Chain support isnt good enough, we need to check the more specific details and that is what this struct is for.
	/// </summary>
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	/// <summary>
	/// I know what this is but not how to explain it TODO : Find out eventually
	/// </summary>
	const std::vector<const char*> DeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	/// <summary>
	/// Undocumented
	/// </summary>
	std::vector<VkImage> SwapChainImages;

	/// <summary>
	/// Undocumented
	/// </summary>
	std::vector<VkImageView> SwapChainImageViews;

	/// <summary>
	/// The swapchain
	/// </summary>
	VkSwapchainKHR VulkanSwapchain;

	VkFormat VulkanSwapchainImageFormat;

	VkExtent2D VulkanSwapchainExtent;

	/// <summary>
	/// Call this to initialize something. This will be called before start(); which you can use for events in game.
	/// </summary>
	void Initialize();

	/// <summary>
	/// Assumes things have been initialized and gets called as the first frame rendered.
	/// </summary>
	void Start();

	/// <summary>
	/// This gets called each frame update. Called in main.cpp.
	/// </summary>
	void Update();

	/// <summary>
	/// Cleans up virtually all the memory objects like the GLFWwindow and cursor.
	/// </summary>
	void Cleanup();

	/// <summary>
	/// Uses Application::ApplicationCursor to set the cursor.
	/// </summary>
	void SetCursor();

	/// <summary>
	/// Set the cursor using an existing cursor.
	/// </summary>
	void SetCursor(GLFWcursor* NewCursor);

	/// <summary>
	/// Set the cursor using a GLFW standard shape. (https://www.glfw.org/docs/latest/group__shapes.html)
	/// </summary>
	void SetCursor(int Shape);

	/// <summary>
	/// Set the cursor using a GLFW image.
	/// </summary>
	void SetCursor(GLFWimage* CursorImage);

	/// <summary>
	/// Set the cursor using a GLFW image and set where the click events gets called from using xhot and yhot.
	/// </summary>
	void SetCursor(GLFWimage* CursorImage, int xhot, int yhot);

	/// <summary>
	/// Set the cursor using a path to some image.
	/// </summary>
	void SetCursor(const char* CursorImagePath);

	/// <summary>
	/// Set the cursor path to some image and set where the click events gets called from using xhot and yhot.	
	/// </summary>
	void SetCursor(const char* CursorImagePath, int xhot, int yhot);

	// ------------------------------ Initialization Functions ---

	/// <summary>
	/// Gets called in Application::Initialize(); Initializes Vulkan which is the rendering API
	/// </summary>
	void InitializeVulkan();

	/// <summary>
	/// Gets called in Application::Initialize(); Initializes the GLFW window.
	/// </summary>
	void InitializeWindow();

	/// <summary>
	/// Gets called in Application::Initialize(); Initializes the GLFW cursor.
	/// </summary>
	void InitializeCursor();

	// ------------------------------ Vulkan ---

	/// <summary>
	/// This function creats a Vulkan instance and configures it.
	/// </summary>
	void CreateVulkanInstance();
	
	/// <summary>
	/// Function for creating the swap chain.
	/// </summary>
	void VulkanCreateSwapChain();

	/// <summary>
	/// This function chooses which device to use. 
	/// </summary>
	void VulkanPickPhysicalDevice();

	/// <summary>
	/// Undocumented
	/// </summary>
	void VulkanCreateLogicalDevice();

	/// <summary>
	/// Undocumented
	/// </summary>
	void VulkanCreateSurface();

	/// <summary>
	/// Undocumented
	/// </summary>
	void VulkanCreateImageViews();

	/// <summary>
	/// Pain.
	/// </summary>
	void VulkanCreateGraphicsPipeline();

	/// <summary>
	/// Checks for validation layer support. Used for debugging.
	/// </summary>
	/// <returns></returns>
	bool CheckValidationLayerSupport();

	/// <summary>
	/// This bool checks the input device if it has some extensions we are going to need. 
	/// </summary>
	/// <param name="_Device"></param>
	/// <returns></returns>
	bool CheckDeviceExtensionSupport(VkPhysicalDevice _Device);

	/// <summary>
	/// bool which checks if a device is compatible with some features.
	/// </summary>
	/// <param name="_Device"></param>
	/// <returns></returns>
	bool isDeviceSuitable(VkPhysicalDevice _Device); // TODO : !IMPORTANT! https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_and_queue_families - rateDeviceSustainability();, let the user choose or pick the first device and let the user change afterwards.

	/// <summary>
	/// This function chooses the swap extent.
	/// </summary>
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities);

	/// <summary>
	/// This function chooses the swap surface format.
	/// </summary>
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats);	
	
	/// <summary>
	/// This function chooses the swap present mode.
	/// </summary>
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes);

	/// <summary>
	/// Undocumented
	/// </summary>
	VulkanQueueFamilyIndices VulkanFindQueueFamilies(VkPhysicalDevice _Device);

	/// <summary>
	/// Undocumented
	/// </summary>
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice _Device);


};

// HERE YOU WILL SEE FUTURE TODOS WHICH I CANT PLACE ANYWHERE ELSE

/*  - PERFORMANCE -
	Note that it's very likely that these end up being the same queue family 
	after all, but throughout the program we will treat them as if they were 
	separate queues for a uniform approach. Nevertheless, you could add logic
	to explicitly prefer a physical device that supports drawing and 
	presentation in the same queue for improved performance.
	https://vulkan-tutorial.com/en/Drawing_a_triangle/Presentation/Window_surface

	Use VK_PRESENT_MODE_MAILBOX_KHR due to something that looks like 
	'tripple buffering' for more frames, energy inefficient though so 
	VK_PRESENT_MODE_FIFO_KHR is the one it defaults to. Of course we check if 
	it exists, but I thought it would be best if i wrote this down.
	Find -> "Presentation Mode" https://vulkan-tutorial.com/en/Drawing_a_triangle/Presentation/Swap_chain#page_Choosing-the-right-settings-for-the-swap-chain
*/