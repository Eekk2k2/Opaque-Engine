#pragma once

#include <iostream>
#include <optional>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <set>
#include <fstream>
#include <string>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Vulkan/OEVulkan.h"
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
	/// Function to get the whole thing going. Call this in main.cpp to start the program.
	/// </summary>
	void Run();

private:

	// ------------------------------ Application ---

	/// <summary>
	/// The vulkan instance, id recommend looking through the code, as this is where most of the 
	/// graphical stuff is done. Class only exists to make the main application smaller and less
	/// intimidating.
	/// </summary>
	OEVulkan::OEVulkanHandler OpaqueEngineVulkanHandler;

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