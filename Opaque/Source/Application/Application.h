#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

#include "Debug/Debug.h"

#pragma once
class Application
{
public:

	// ------------------------------ MOVE TO CAMERA ONCE IMPLEMENTED --- TODO ::

	/// <summary>
	/// The x size of the applcation window (*ApplcationWindow from Applcation.h) This value is a constant.
	/// </summary>
	const uint32_t ApplcationWidth = 1920;

	/// <summary>
	/// The y size of the applcation window (*ApplcationWindow from Applcation.h) This value is a constant.
	/// </summary>
	const uint32_t ApplcationHeight = 1080;

	// ------------------------------ APPLICATION ---

	/// <summary>
	/// Each time the application name is needed this value will be referenced. This value is a constant.
	/// </summary>
	const char* ApplicationName = "Unnamed Application";

	/// <summary>
	/// The GLFW window object of this application.
	/// </summary>
	GLFWwindow* ApplicationWindow;

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

	// ------------------------------ APPLICATION ---

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

	// ------------------------------ Update Functions ---
};

