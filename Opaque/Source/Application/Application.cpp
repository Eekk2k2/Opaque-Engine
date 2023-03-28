#include "Application.h"

// BIG TODO : Move every vulkan functon to a different file; too cluttered.

// ------------------------------ Callbacks ---

// ------------------------------ Public ---

void Application::Run()
{
	Initialize();
	Start();
	Update();
	Cleanup();
}

// ------------------------------ Private ---

void Application::Initialize()
{
	InitializeWindow();
	InitializeCursor();
	//InitializeIcon(); TODO : Implement

	InitializeVulkan();
}

void Application::Start()
{

}

void Application::Update()
{
	while (!glfwWindowShouldClose(ApplicationWindow)) // Each frame in here
	{
		glfwPollEvents();
	}
}

void Application::Cleanup()
{
	// OEVulkan cleanup
	OpaqueEngineVulkanHandler.OEVulkanCleanup();

	// GLFW
	glfwDestroyCursor(ApplicationCursor);
	glfwDestroyWindow(ApplicationWindow);

	glfwTerminate();
}

void Application::SetCursor()
{
	glfwSetCursor(ApplicationWindow, ApplicationCursor);
}

void Application::SetCursor(GLFWcursor* NewCursor)
{
	ApplicationCursor = NewCursor;

	glfwSetCursor(ApplicationWindow, ApplicationCursor);
}

void Application::SetCursor(int Shape)
{
	ApplicationCursor = glfwCreateStandardCursor(Shape);

	glfwSetCursor(ApplicationWindow, ApplicationCursor);
}

void Application::SetCursor(GLFWimage* CursorImage)
{
	ApplicationCursor = glfwCreateCursor(CursorImage, 0, 0);

	glfwSetCursor(ApplicationWindow, ApplicationCursor);
}

void Application::SetCursor(GLFWimage* CursorImage, int xhot, int yhot)
{
	ApplicationCursor = glfwCreateCursor(CursorImage, xhot, yhot);

	glfwSetCursor(ApplicationWindow, ApplicationCursor);
}

void Application::SetCursor(const char* CursorImagePath)
{
	std::cout << "Application::SetCursor(const char* CursorImagePath) not supported yet || Line: " << __LINE__ << std::endl; // TODO : Find out why glfwImageRead() doesnt work / exist
}

void Application::SetCursor(const char* CursorImagePath, int xhot, int yhot)
{
	std::cout << "Application::SetCursor(const char* CursorImagePath, int xhot, int yhot) Line: " << __LINE__ << std::endl; // TODO : Find out why glfwImageRead() doesnt work / exist
}

// ------------------------------ Initialization Functions ---

void Application::InitializeVulkan()
{
	OEVulkan::OEVulkanHandlerCreateInfo OpaqueEngineVulkanHandlerCreateInfo{};
	OpaqueEngineVulkanHandlerCreateInfo.ApplicationWidth = this->ApplicationWidth;
	OpaqueEngineVulkanHandlerCreateInfo.ApplicationHeight = this->ApplicationHeight;
	OpaqueEngineVulkanHandlerCreateInfo.ApplicationName = this->ApplicationName;

	OpaqueEngineVulkanHandlerCreateInfo.VulkanAPIVersion = VK_API_VERSION_1_3;

	OpaqueEngineVulkanHandlerCreateInfo.pApplicationWindow = this->ApplicationWindow;

	OEVulkan::General::InitializeVulkan(OpaqueEngineVulkanHandler, OpaqueEngineVulkanHandlerCreateInfo);
}

void Application::InitializeWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Since we are using Vulkan we need to tell GLFW
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disables resizing. Change this if you want. The callback is FramebufferResizeCallback(GLFWwindow* window, int width, int height);

	ApplicationWindow = glfwCreateWindow(ApplicationWidth, ApplicationHeight, ApplicationName, nullptr, nullptr); // Sets the ApplicationWindow variable. TODO : When the camera gets implements errors are going to happen here
}

void Application::InitializeCursor()
{
	SetCursor(GLFW_ARROW_CURSOR);
}