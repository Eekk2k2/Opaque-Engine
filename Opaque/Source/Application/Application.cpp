#include "Application.h"

// BIG TODO : Move every vulkan functon to a different file; too cluttered.

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
	// Vulkan
	for (auto ImageView : SwapChainImageViews)
		vkDestroyImageView(VulkanDevice, ImageView, nullptr);

	vkDestroySwapchainKHR(VulkanDevice, VulkanSwapchain, nullptr);
	vkDestroyDevice(VulkanDevice, nullptr);
	vkDestroySurfaceKHR(VulkanInstance, VulkanSurface, nullptr);
	vkDestroyInstance(VulkanInstance, nullptr);

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

	OEVulkan::OEVulkanHandler OpaqueEngineVulkanHandler = OEVulkan::OEVulkanHandler();
	OEVulkan::General::InitializeVulkan(OpaqueEngineVulkanHandler, OpaqueEngineVulkanHandlerCreateInfo);

	//CreateVulkanInstance();
	 //SetupDebugMessenger(); TODO
	//VulkanCreateSurface();
	//VulkanPickPhysicalDevice();
	//VulkanCreateLogicalDevice();
	//VulkanCreateSwapChain();
	//VulkanCreateImageViews();
	//VulkanCreateGraphicsPipeline();
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

// ------------------------------ Update Functions ---

// ------------------------------ Other Functions ---
void Application::CreateVulkanInstance()
{
	if (EnableValidationLayers && !CheckValidationLayerSupport()) { throw std::runtime_error("Vulkan validation layers requested, but for some reason not available."); } // Validation layers is used for various debugging processes

	VkApplicationInfo VulkanApplicationInfo{};

	VulkanApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	VulkanApplicationInfo.pApplicationName = ApplicationName;
	VulkanApplicationInfo.pEngineName = "Opaque";

	VulkanApplicationInfo.apiVersion = VK_API_VERSION_1_3; // Edit this as a way to fix : pCreateInfo->imageFormat VK_FORMAT_B8G8R8A8_SRGB with tiling VK_IMAGE_TILING_OPTIMAL has no supported format features on this physical device

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
	} else { CreateVulkanApplicationInfo.enabledLayerCount = 0; }

	// Now we create the instance
	if (vkCreateInstance(&CreateVulkanApplicationInfo, nullptr, &VulkanInstance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create a Vulkan Instance!"); /* TODO: debug.log(); */
}

void Application::VulkanCreateSwapChain()
{
	SwapChainSupportDetails SwapChainSupport = QuerySwapChainSupport(VulkanPhysicalDevice);

	VkSurfaceFormatKHR SurfaceFormat = ChooseSwapSurfaceFormat(SwapChainSupport.Formats);
	VkPresentModeKHR PresentMode = ChooseSwapPresentMode(SwapChainSupport.PresentModes);
	VkExtent2D Extent = ChooseSwapExtent(SwapChainSupport.Capabilities);

	uint32_t ImageCount = SwapChainSupport.Capabilities.minImageCount + 1;

	if (SwapChainSupport.Capabilities.maxImageCount > 0 && ImageCount > SwapChainSupport.Capabilities.maxImageCount)
		ImageCount = SwapChainSupport.Capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR SwapchainCreateInfo{};
	SwapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapchainCreateInfo.surface = VulkanSurface;
	SwapchainCreateInfo.minImageCount = ImageCount;
	SwapchainCreateInfo.imageFormat = SurfaceFormat.format;
	SwapchainCreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
	SwapchainCreateInfo.imageExtent = Extent;
	SwapchainCreateInfo.imageArrayLayers = 1;
	SwapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	VulkanQueueFamilyIndices QueueIndices = VulkanFindQueueFamilies(VulkanPhysicalDevice);
	uint32_t QueueFamilyIndices[] = { QueueIndices.GraphicsFamily.has_value(), QueueIndices.PresentFamily.has_value() };

	if (QueueIndices.GraphicsFamily != QueueIndices.PresentFamily)
	{
		SwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		SwapchainCreateInfo.queueFamilyIndexCount = 2;
		SwapchainCreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
	}
	else
	{
		SwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		SwapchainCreateInfo.queueFamilyIndexCount = 0; // Not needed
		SwapchainCreateInfo.pQueueFamilyIndices = nullptr;// Not needed
	}

	SwapchainCreateInfo.preTransform = SwapChainSupport.Capabilities.currentTransform;
	SwapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	SwapchainCreateInfo.presentMode = PresentMode;
	SwapchainCreateInfo.clipped = VK_TRUE; // Clipping
	SwapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;


	// Finally creates the swap chain
	if (vkCreateSwapchainKHR(VulkanDevice, &SwapchainCreateInfo, nullptr, &VulkanSwapchain) != VK_SUCCESS)
		throw std::runtime_error("Failed to create swap chain!");
	
	vkGetSwapchainImagesKHR(VulkanDevice, VulkanSwapchain, &ImageCount, nullptr);
	SwapChainImages.reserve(ImageCount);
	vkGetSwapchainImagesKHR(VulkanDevice, VulkanSwapchain, &ImageCount, SwapChainImages.data());

	VulkanSwapchainImageFormat = SurfaceFormat.format;
	VulkanSwapchainExtent = Extent;
}

void Application::VulkanPickPhysicalDevice()
{
	uint32_t DeviceCount = 0;
	vkEnumeratePhysicalDevices(VulkanInstance, &DeviceCount, nullptr);
	if (DeviceCount == 0) { throw std::runtime_error("Either failed to find or no GPUs with Vulkan support. || LINE : " + __LINE__);
	}

	std::vector<VkPhysicalDevice> Devices(DeviceCount);
	vkEnumeratePhysicalDevices(VulkanInstance, &DeviceCount, Devices.data());

	for (const auto& Device : Devices)
	{
		if (isDeviceSuitable(Device)) { VulkanPhysicalDevice = Device; break; }
		else { throw std::runtime_error("No suitabke"); }
	}

	if (VulkanPhysicalDevice == VK_NULL_HANDLE) { throw std::runtime_error("Failed to find a suitable GPU || LINE : " + __LINE__); }
}

void Application::VulkanCreateLogicalDevice()
{
	VulkanQueueFamilyIndices QueueIndices = VulkanFindQueueFamilies(VulkanPhysicalDevice);

	std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos;
	std::set<uint32_t> UniqueQueueFamilies = { 
		QueueIndices.GraphicsFamily.value(), 
		QueueIndices.PresentFamily.value() 
	};

	float QueuePriority = 1.0f; // Range between 0.0 and 1.0
	for (uint32_t QueueFamily : UniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo QueueCreateInfo{};
		QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		QueueCreateInfo.queueFamilyIndex = QueueFamily;
		QueueCreateInfo.queueCount = 1;
		QueueCreateInfo.pQueuePriorities = &QueuePriority;
		QueueCreateInfos.push_back(QueueCreateInfo);
	}
	
	VkPhysicalDeviceFeatures DeviceFeatures{};

	VkDeviceCreateInfo DeviceCreateInfo{};
	DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	DeviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(QueueCreateInfos.size());
	DeviceCreateInfo.pQueueCreateInfos = QueueCreateInfos.data();
	DeviceCreateInfo.pEnabledFeatures = &DeviceFeatures;
	DeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
	DeviceCreateInfo.ppEnabledExtensionNames = DeviceExtensions.data();

	if (EnableValidationLayers)
	{
		DeviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
		DeviceCreateInfo.ppEnabledLayerNames = ValidationLayers.data();
	}
	else { DeviceCreateInfo.enabledLayerCount = 0; }

	// Now we create the device
	if (vkCreateDevice(VulkanPhysicalDevice, &DeviceCreateInfo, nullptr, &VulkanDevice) != VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device. || LINE : " + __LINE__);

	vkGetDeviceQueue(VulkanDevice, QueueIndices.GraphicsFamily.value(), 0, &GraphicsQueue);
	vkGetDeviceQueue(VulkanDevice, QueueIndices.PresentFamily.value(), 0, &PresentQueue);
}

void Application::VulkanCreateSurface()
{
	// Gives data to the VkSurfaceKHR Aplication::VulkanSurface;
	if (glfwCreateWindowSurface(VulkanInstance, ApplicationWindow, nullptr, &VulkanSurface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create a window surface. || LINE : " + __LINE__);
}

void Application::VulkanCreateImageViews()
{
	SwapChainImageViews.resize(SwapChainImages.size());

	for (size_t i = 0; i < SwapChainImages.size(); i++)
	{
		VkImageViewCreateInfo ImageViewCreateInfo{};
		ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ImageViewCreateInfo.image = SwapChainImages[i];
		ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // Can be 1D, 2D, 3D, Cubemap, etc, but since this is shown to the screen we want it to show a 2D image.
		ImageViewCreateInfo.format = VulkanSwapchainImageFormat;

		ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; // RED
		ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY; // GREED
		ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY; // BLUE
		ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY; // ALPHA

		ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		ImageViewCreateInfo.subresourceRange.levelCount = 1;
		ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		ImageViewCreateInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(VulkanDevice, &ImageViewCreateInfo, nullptr, &SwapChainImageViews[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create image views!");
		}
	}
}


void Application::VulkanCreateGraphicsPipeline()
{
	/* SHADER STAGE */

	std::vector<std::string> ShaderFilePaths = {
		"E:\\Projects\\.vulkan\\Opaque\\Opaque\\Source\\Shaders\\Compiled\\DefaultShaderFragment.spv",  // Where they are located to me, persitent data paths is not a thing yet
		"E:\\Projects\\.vulkan\\Opaque\\Opaque\\Source\\Shaders\\Compiled\\DefaultShaderVertex.spv",  // Where they are located to me, persitent data paths is not a thing yet
	};

	std::vector<char> VertexShaderCodeBuffer = ShaderManager::ReadShaderFile(ShaderFilePaths[0]);
	std::vector<char> FragmentShaderCodeBuffer = ShaderManager::ReadShaderFile(ShaderFilePaths[1]);

	VkShaderModule VertexShaderModule = ShaderManager::VulkanCreateShaderModule(VertexShaderCodeBuffer, VulkanDevice);
	VkShaderModule FragmentShaderModule = ShaderManager::VulkanCreateShaderModule(FragmentShaderCodeBuffer, VulkanDevice);

	VkPipelineShaderStageCreateInfo VertexShaderStageInfo{};
	VertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	VertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	VertexShaderStageInfo.module = VertexShaderModule;
	VertexShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo FragmentShaderStageInfo{};
	FragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	FragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	FragmentShaderStageInfo.module = FragmentShaderModule;
	FragmentShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo ShaderStagesCreateInfo[] = { VertexShaderStageInfo, FragmentShaderStageInfo };

	/* VERTEX INPUT */

	VkPipelineVertexInputStateCreateInfo VertexInputCreateInfo{};
	VertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	VertexInputCreateInfo.pVertexBindingDescriptions = nullptr;
	VertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	VertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;

	/* INPUT ASSEMBLY */

	VkPipelineInputAssemblyStateCreateInfo InputAssemblyCreateInfo{};
	InputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	InputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	InputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

	/* VIEWPORT AND SCISSORS */

	VkViewport VulkanViewport{};
	VulkanViewport.x = 0.0f;
	VulkanViewport.y = 0.0f;
	VulkanViewport.width = (float)VulkanSwapchainExtent.width;
	VulkanViewport.height = (float)VulkanSwapchainExtent.height;
	VulkanViewport.minDepth = 0.0f;
	VulkanViewport.maxDepth = 1.0f;

	VkRect2D VulkanScissor{};
	VulkanScissor.offset = { 0, 0 };
	VulkanScissor.extent = VulkanSwapchainExtent;

	std::vector<VkDynamicState> DynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo DynamicStateCreateInfo{};
	DynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	DynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(DynamicStates.size());
	DynamicStateCreateInfo.pDynamicStates = DynamicStates.data();

	VkPipelineViewportStateCreateInfo ViewportStateCreateInfo{};
	ViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

	ViewportStateCreateInfo.viewportCount = 1;
	ViewportStateCreateInfo.pViewports = &VulkanViewport;

	ViewportStateCreateInfo.scissorCount = 1;
	ViewportStateCreateInfo.pScissors = &VulkanScissor;

	/* Rasterizer */ 

	VkPipelineRasterizationStateCreateInfo RasterizerCreateInfo{};
	RasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	RasterizerCreateInfo.depthClampEnable = VK_FALSE;
	RasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	RasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	RasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	//RasterizerCreateInfo.lineWidth = 1.0f;
	RasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;;
	RasterizerCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	RasterizerCreateInfo.depthBiasEnable = VK_FALSE;

	/* Multisampling */

	VkPipelineMultisampleStateCreateInfo MultisamplingCreateInfo{};
	MultisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	MultisamplingCreateInfo.sampleShadingEnable = VK_FALSE;
	MultisamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	MultisamplingCreateInfo.minSampleShading = 1.0f; // Optional
	MultisamplingCreateInfo.pSampleMask = nullptr; // Optional
	MultisamplingCreateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
	MultisamplingCreateInfo.alphaToOneEnable = VK_FALSE; // Optional
	
	/* Depth and stencil testing */
	
	// No codes exists due to there not being any stencil and depth buffers.

	/* Color blending */

	VkPipelineColorBlendAttachmentState ColorBlendAttachmentState{};
	ColorBlendAttachmentState.colorWriteMask =	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	ColorBlendAttachmentState.blendEnable = VK_TRUE;
	ColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	ColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	ColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	ColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	ColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	ColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;


	vkDestroyShaderModule(VulkanDevice, VertexShaderModule, nullptr);
	vkDestroyShaderModule(VulkanDevice, FragmentShaderModule , nullptr);
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

bool Application::CheckDeviceExtensionSupport(VkPhysicalDevice _Device)
{
	uint32_t ExtensionsCount;
	vkEnumerateDeviceExtensionProperties(_Device, nullptr, &ExtensionsCount, nullptr);

	std::vector<VkExtensionProperties> AvailableExtensions(ExtensionsCount);
	vkEnumerateDeviceExtensionProperties(_Device, nullptr, &ExtensionsCount, AvailableExtensions.data());

	std::set<std::string> RequiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());

	for (const auto& Extension : AvailableExtensions) { RequiredExtensions.erase(Extension.extensionName); }

	return RequiredExtensions.empty();
}

bool Application::isDeviceSuitable(VkPhysicalDevice _Device)
{
	Application::VulkanQueueFamilyIndices QueueIndices = VulkanFindQueueFamilies(_Device);
	bool ExtensionsSupported = CheckDeviceExtensionSupport(_Device);
	bool SwapChainAdequate = false;
	if (ExtensionsSupported)
	{
 		SwapChainSupportDetails SwapChainSupport = QuerySwapChainSupport(_Device);
		SwapChainAdequate = !SwapChainSupport.Formats.empty() && !SwapChainSupport.PresentModes.empty(); // Failed to find a suitable GPU
	}
	// ... more checks

	return QueueIndices.isComplete() && ExtensionsSupported && SwapChainAdequate;
}

VkExtent2D Application::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities)
{
	if (Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return Capabilities.currentExtent;
	} 
	else
	{
		int width, height;
		glfwGetFramebufferSize(ApplicationWindow, &width, &height);

		VkExtent2D ActualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		ActualExtent.width = std::clamp(ActualExtent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
		ActualExtent.height = std::clamp(ActualExtent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);

		return ActualExtent;
	}
}

VkSurfaceFormatKHR Application::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats)
{
	for (const auto& AvailableFormat : AvailableFormats)
	{
		if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && AvailableFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) // Preffered
			return AvailableFormat;
	}

	return AvailableFormats[0]; // If the preffered one isnt here for us we just return the first option, so if you are having issues related to wierd color space and color values look here.
}

VkPresentModeKHR Application::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes)
{	
	for (const auto& AvailablePresentMode : AvailablePresentModes)
	{
		if (AvailablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) // Preffered; more frames traded for higher energy cost
		{
			return AvailablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR; 
}

Application::VulkanQueueFamilyIndices Application::VulkanFindQueueFamilies(VkPhysicalDevice _Device)
{
	VulkanQueueFamilyIndices QueueIndices;

	uint32_t QueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_Device, &QueueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(_Device, &QueueFamilyCount, QueueFamilies.data());
	
	int i = 0;
	for (const auto& QueueFamily : QueueFamilies)
	{
		if (QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) { QueueIndices.GraphicsFamily = i; }

		VkBool32 PresentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(_Device, i, VulkanSurface, &PresentSupport);

		if (PresentSupport) { QueueIndices.PresentFamily = i; }

		if (QueueIndices.isComplete()) { break; }
		i++;
	}

	return QueueIndices;
}

Application::SwapChainSupportDetails Application::QuerySwapChainSupport(VkPhysicalDevice _Device)
{
	SwapChainSupportDetails Details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_Device, VulkanSurface, &Details.Capabilities);

	uint32_t FormatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, VulkanSurface, &FormatCount, nullptr);

	if (FormatCount != 0)
	{
		Details.Formats.resize(FormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, VulkanSurface, &FormatCount, Details.Formats.data());
	}

	uint32_t PresentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, VulkanSurface, &PresentModeCount, nullptr);

	if (PresentModeCount != 0)
	{
		Details.PresentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, VulkanSurface, &PresentModeCount, Details.PresentModes.data());
	}

	return Details;
}
