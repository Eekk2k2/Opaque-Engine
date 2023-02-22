#include "Application.h"

// ------------------------------ Callbacks ---

/// <summary>
/// This callback gets called each time the frame gets resized.
/// </summary>
/// <param name="window"></param>
/// <param name="width"></param>
/// <param name="height"></param>
//void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}

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
	CreateVulkanInstance();
	// SetupDebugMessenger(); TODO
	// VulkanPickpPhysicalDevice();
}

void Application::InitializeWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Since we are using Vulkan we need to tell GLFW
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disables resizing. Change this if you want. The callback is FramebufferResizeCallback(GLFWwindow* window, int width, int height);

	ApplicationWindow = glfwCreateWindow(ApplcationWidth, ApplcationHeight, ApplicationName, nullptr, nullptr); // Sets the ApplicationWindow variable. TODO : When the camera gets implements errors are going to happen here
}

void Application::InitializeCursor()
{
	SetCursor(GLFW_ARROW_CURSOR);
}

// ------------------------------ Update Functions ---

// ------------------------------ Other Functions ---

void Application::CreateVulkanInstance()
{
	if (EnableValidationLayers && !CheckValidationLayerSupport()) { throw std::runtime_error("Vulkan validation layers requested, but for some reason not available."); } // Validation layers is used for various debugging processes

	VkApplicationInfo VulkanApplicationInfo{};

	VulkanApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	VulkanApplicationInfo.pApplicationName = ApplicationName;
	VulkanApplicationInfo.pEngineName = "Opaque";

	VkInstanceCreateInfo CreateVulkanApplicationInfo{};
	CreateVulkanApplicationInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateVulkanApplicationInfo.pApplicationInfo = &VulkanApplicationInfo;

	uint32_t GLFWExtensionsCount = 0;
	const char** GLFWExtensions;

	GLFWExtensions = glfwGetRequiredInstanceExtensions(&GLFWExtensionsCount);

	CreateVulkanApplicationInfo.enabledExtensionCount = GLFWExtensionsCount;
	CreateVulkanApplicationInfo.ppEnabledExtensionNames = GLFWExtensions;

	if (EnableValidationLayers)
	{
		CreateVulkanApplicationInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
		CreateVulkanApplicationInfo.ppEnabledLayerNames = ValidationLayers.data();
	}
	else { CreateVulkanApplicationInfo.enabledLayerCount = 0; }


	if (vkCreateInstance(&CreateVulkanApplicationInfo, nullptr, &VulkanInstance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create a Vulkan Instance!"); /* TODO: debug.log(); */
}

bool Application::CheckValidationLayerSupport()
{
	uint32_t VulkanValidationLayerCount = 0;
	vkEnumerateInstanceLayerProperties(&VulkanValidationLayerCount, nullptr);

	std::vector<VkLayerProperties> AvailableVulkanValidationLayers(VulkanValidationLayerCount);
	vkEnumerateInstanceLayerProperties(&VulkanValidationLayerCount, AvailableVulkanValidationLayers.data());

	for (const char* VulkanValidationLayerName : ValidationLayers)
	{
		bool VulkanValidationLayerFound = false;

		for (const auto& VulkanValidationLayersProperties : AvailableVulkanValidationLayers)
		{
			if (strcmp(VulkanValidationLayerName, VulkanValidationLayersProperties.layerName) == 0)
			{
				VulkanValidationLayerFound = true;
				break;
			}
		}

		if (!VulkanValidationLayerFound) { return false; }
	}

	return true;
}

bool Application::isDeviceSuitable(VkPhysicalDevice _Device)
{
	VkPhysicalDeviceProperties _DeviceProperties;
	VkPhysicalDeviceFeatures _DeviceFeatures;

	vkGetPhysicalDeviceProperties(_Device, &_DeviceProperties);
	vkGetPhysicalDeviceFeatures(_Device, &_DeviceFeatures);

	return true;
}

void Application::VulkanPickPhysicalInstance()
{
	VkPhysicalDevice VulkanPhysicalDevice = VK_NULL_HANDLE;

	uint32_t DeviceCount = 0;
	vkEnumeratePhysicalDevices(VulkanInstance, &DeviceCount, nullptr);
	if (!DeviceCount) { throw std::runtime_error("Either failed to find or no GPUs with Vulkan support."); }

	std::vector<VkPhysicalDevice> Devices(DeviceCount);
	vkEnumeratePhysicalDevices(VulkanInstance, &DeviceCount, Devices.data());


	for (const auto& Device : Devices)
	{
		if (isDeviceSuitable(Device)) { VulkanPhysicalDevice = Device; break; }
		else { throw std::runtime_error("Failed to find a suitable GPU."); }
	}
}

Application::VulkanQueueFamilyIndices Application::VulkanFindQueueFamilies(VkPhysicalDevice _Device)
{
	VulkanQueueFamilyIndices QueueIndices;

	uint32_t QueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_Device, &QueueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(_Device, &QueueFamilyCount, QueueFamilies.data());

	return QueueIndices;
}