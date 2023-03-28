#include "OEVulkan.h"

OEVulkan::OEVulkanHandler::OEVulkanHandler() {} // Remember to call InitializeVulkan when using this!

OEVulkan::OEVulkanHandler::OEVulkanHandler(OEVulkanHandlerCreateInfo CreateInfo) {}










/* GENERAL */

void OEVulkan::General::InitializeVulkan
(
	OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler,
	OEVulkan::OEVulkanHandlerCreateInfo OpaqueEngineVulkanHandlerCreateInfo
)
{
	OpaqueEngineVulkanHandler.ApplicationWidth = OpaqueEngineVulkanHandlerCreateInfo.ApplicationWidth | 1;
	OpaqueEngineVulkanHandler.ApplicationHeight = OpaqueEngineVulkanHandlerCreateInfo.ApplicationHeight;
	OpaqueEngineVulkanHandler.ApplicationName = OpaqueEngineVulkanHandlerCreateInfo.ApplicationName;

	OpaqueEngineVulkanHandler.pApplicationWindow = OpaqueEngineVulkanHandlerCreateInfo.pApplicationWindow;

	OpaqueEngineVulkanHandler.VulkanAPIVersion = OpaqueEngineVulkanHandlerCreateInfo.VulkanAPIVersion;

	OEVulkan::General::CreateInstance(OpaqueEngineVulkanHandler);
	OEVulkan::General::CreateSurface(OpaqueEngineVulkanHandler);
	OEVulkan::Device::PickPhysicalDevice(OpaqueEngineVulkanHandler);
	OEVulkan::Device::CreateLogicalDevice(OpaqueEngineVulkanHandler);
	OEVulkan::Swapchain::CreateSwapchain(OpaqueEngineVulkanHandler);
	OEVulkan::Swapchain::CreateImageViews(OpaqueEngineVulkanHandler);
	OEVulkan::GraphicsPipeline::CreateGraphicsPipeline(OpaqueEngineVulkanHandler);
}

void OEVulkan::General::CreateInstance(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler)
{
	if (OpaqueEngineVulkanHandler.EnableValidationLayers && !OEVulkan::ValidationLayers::CheckValidationLayerSupport(OpaqueEngineVulkanHandler)) { throw std::runtime_error("Vulkan validation layers requested, but for some reason not available."); } // Validation layers is used for various debugging processes

	VkApplicationInfo VulkanApplicationInfo{};

	VulkanApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	VulkanApplicationInfo.pApplicationName = OpaqueEngineVulkanHandler.ApplicationName.c_str();
	VulkanApplicationInfo.pEngineName = "Opaque";

	// Edit this as a way to fix : pCreateInfo->imageFormat VK_FORMAT_B8G8R8A8_SRGB with tiling VK_IMAGE_TILING_OPTIMAL has no supported format features on this physical device
	VulkanApplicationInfo.apiVersion = OpaqueEngineVulkanHandler.VulkanAPIVersion;

	VkInstanceCreateInfo CreateVulkanApplicationInfo{};
	CreateVulkanApplicationInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateVulkanApplicationInfo.pApplicationInfo = &VulkanApplicationInfo;

	uint32_t GLFWExtensionsCount = 0;
	const char** GLFWExtensions;

	GLFWExtensions = glfwGetRequiredInstanceExtensions(&GLFWExtensionsCount);

	CreateVulkanApplicationInfo.enabledExtensionCount = GLFWExtensionsCount;
	CreateVulkanApplicationInfo.ppEnabledExtensionNames = GLFWExtensions;

	if (OpaqueEngineVulkanHandler.EnableValidationLayers)
	{
		CreateVulkanApplicationInfo.enabledLayerCount = static_cast<uint32_t>(OpaqueEngineVulkanHandler.ValidationLayers.size());
		CreateVulkanApplicationInfo.ppEnabledLayerNames = OpaqueEngineVulkanHandler.ValidationLayers.data();
	}
	else { CreateVulkanApplicationInfo.enabledLayerCount = 0; }

	// Now we create the instance
	if (vkCreateInstance(&CreateVulkanApplicationInfo, nullptr, &OpaqueEngineVulkanHandler.VulkanInstance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create a Vulkan Instance!"); /* TODO: debug.log(); */
}

void OEVulkan::General::CreateSurface(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler)
{
	if (glfwCreateWindowSurface(OpaqueEngineVulkanHandler.VulkanInstance, OpaqueEngineVulkanHandler.pApplicationWindow, nullptr, &OpaqueEngineVulkanHandler.VulkanSurfaceKHR))
		throw std::runtime_error("Failed to create a window surface.");
}

void OEVulkan::Swapchain::CreateImageViews(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler)
{
	OpaqueEngineVulkanHandler.SwapchainImageViews.resize(OpaqueEngineVulkanHandler.SwapchainImages.size());

	for (size_t i = 0; i < OpaqueEngineVulkanHandler.SwapchainImages.size(); i++)
	{
		VkImageViewCreateInfo ImageViewCreateInfo{};
		ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ImageViewCreateInfo.image = OpaqueEngineVulkanHandler.SwapchainImages[i];
		ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // Can be 1D, 2D, 3D, Cubemap, etc, but since this is shown to the screen we want it to show a 2D image.
		ImageViewCreateInfo.format = OpaqueEngineVulkanHandler.VulkanSwapchainImageFormat;

		ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; // RED
		ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY; // GREED
		ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY; // BLUE
		ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY; // ALPHA

		ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		ImageViewCreateInfo.subresourceRange.levelCount = 1;
		ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		ImageViewCreateInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(OpaqueEngineVulkanHandler.VulkanDevice, &ImageViewCreateInfo, nullptr, &OpaqueEngineVulkanHandler.SwapchainImageViews[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create image views!");
		}
	}
}









/* VALIDATION LAYERS */

bool OEVulkan::ValidationLayers::CheckValidationLayerSupport(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler)
{
	uint32_t VulkanValidationLayerCount = 0;
	vkEnumerateInstanceLayerProperties(&VulkanValidationLayerCount, nullptr);

	std::vector<VkLayerProperties> AvailableVulkanValidationLayers(VulkanValidationLayerCount);
	vkEnumerateInstanceLayerProperties(&VulkanValidationLayerCount, AvailableVulkanValidationLayers.data());

	for (const char* VulkanValidationLayerName : OpaqueEngineVulkanHandler.ValidationLayers)
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










/* DEVICE */

void OEVulkan::Device::PickPhysicalDevice(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler)
{
	uint32_t DeviceCount = 0;
	vkEnumeratePhysicalDevices(OpaqueEngineVulkanHandler.VulkanInstance, &DeviceCount, nullptr);
	if (DeviceCount == 0)
		throw std::runtime_error("Either failed to find or no GPUs with Vulkan support.");

	std::vector<VkPhysicalDevice> Devices(DeviceCount);
	vkEnumeratePhysicalDevices(OpaqueEngineVulkanHandler.VulkanInstance, &DeviceCount, Devices.data());

	for (const auto& Device : Devices)
	{
		if (OEVulkan::Device::isDeviceSuitable(Device, OpaqueEngineVulkanHandler)) { OpaqueEngineVulkanHandler.VulkanPhysicalDevice = Device; break; }
		else { throw std::runtime_error("No suitabke"); }
	}

	if (OpaqueEngineVulkanHandler.VulkanPhysicalDevice == VK_NULL_HANDLE) { throw std::runtime_error("Failed to find a suitable GPU || LINE : " + __LINE__); }
}

void OEVulkan::Device::CreateLogicalDevice(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler)
{
	VulkanQueueFamilyIndices QueueIndices = OEVulkan::Device::FindQueueFamilies(OpaqueEngineVulkanHandler.VulkanPhysicalDevice, OpaqueEngineVulkanHandler.VulkanSurfaceKHR);

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
	DeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(OpaqueEngineVulkanHandler.DeviceExtensions.size());
	DeviceCreateInfo.ppEnabledExtensionNames = OpaqueEngineVulkanHandler.DeviceExtensions.data();

	if (OpaqueEngineVulkanHandler.EnableValidationLayers)
	{
		DeviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(OpaqueEngineVulkanHandler.ValidationLayers.size());
		DeviceCreateInfo.ppEnabledLayerNames = OpaqueEngineVulkanHandler.ValidationLayers.data();
	}
	else { DeviceCreateInfo.enabledLayerCount = 0; }

	// Now we create the device
	if (vkCreateDevice(OpaqueEngineVulkanHandler.VulkanPhysicalDevice, &DeviceCreateInfo, nullptr, &OpaqueEngineVulkanHandler.VulkanDevice) != VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device. || LINE : " + __LINE__);

	vkGetDeviceQueue(OpaqueEngineVulkanHandler.VulkanDevice, QueueIndices.GraphicsFamily.value(), 0, &OpaqueEngineVulkanHandler.GraphicsQueue);
	vkGetDeviceQueue(OpaqueEngineVulkanHandler.VulkanDevice, QueueIndices.PresentFamily.value(), 0, &OpaqueEngineVulkanHandler.PresentQueue);
}

bool OEVulkan::Device::isDeviceSuitable(VkPhysicalDevice _Device, OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler)
{
	OEVulkan::Device::VulkanQueueFamilyIndices QueueIndices = OEVulkan::Device::FindQueueFamilies(_Device, OpaqueEngineVulkanHandler.VulkanSurfaceKHR);
	bool ExtensionsSupported = OEVulkan::Device::CheckDeviceExtensionSupport(_Device, OpaqueEngineVulkanHandler);
	bool SwapChainAdequate = false;
	if (ExtensionsSupported)
	{
		OEVulkan::Swapchain::SwapChainSupportDetails SwapChainSupport = OEVulkan::Swapchain::QuerySwapChainSupport(_Device, OpaqueEngineVulkanHandler.VulkanSurfaceKHR);
		SwapChainAdequate = !SwapChainSupport.Formats.empty() && !SwapChainSupport.PresentModes.empty(); // Failed to find a suitable GPU
	}
	// ... more checks

	return QueueIndices.isComplete() && ExtensionsSupported && SwapChainAdequate;
}

bool OEVulkan::Device::CheckDeviceExtensionSupport(VkPhysicalDevice _Device, OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler)
{
	uint32_t ExtensionsCount;
	vkEnumerateDeviceExtensionProperties(_Device, nullptr, &ExtensionsCount, nullptr);

	std::vector<VkExtensionProperties> AvailableExtensions(ExtensionsCount);
	vkEnumerateDeviceExtensionProperties(_Device, nullptr, &ExtensionsCount, AvailableExtensions.data());

	std::set<std::string> RequiredExtensions(OpaqueEngineVulkanHandler.DeviceExtensions.begin(), OpaqueEngineVulkanHandler.DeviceExtensions.end());

	for (const auto& Extension : AvailableExtensions) { RequiredExtensions.erase(Extension.extensionName); }

	return RequiredExtensions.empty();
}

OEVulkan::Device::VulkanQueueFamilyIndices OEVulkan::Device::FindQueueFamilies(VkPhysicalDevice _Device, VkSurfaceKHR& _VulkanSurface)
{
	OEVulkan::Device::VulkanQueueFamilyIndices QueueIndices;

	uint32_t QueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_Device, &QueueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(_Device, &QueueFamilyCount, QueueFamilies.data());

	int i = 0;
	for (const auto& QueueFamily : QueueFamilies)
	{
		if (QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) { QueueIndices.GraphicsFamily = i; }

		VkBool32 PresentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(_Device, i, _VulkanSurface, &PresentSupport);

		if (PresentSupport) { QueueIndices.PresentFamily = i; }

		if (QueueIndices.isComplete()) { break; }
		i++;
	}

	return QueueIndices;
}










/* SWAPCHAIN */

OEVulkan::Swapchain::SwapChainSupportDetails OEVulkan::Swapchain::QuerySwapChainSupport(VkPhysicalDevice _Device, VkSurfaceKHR& _VulkanSurface)
{
	OEVulkan::Swapchain::SwapChainSupportDetails Details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_Device, _VulkanSurface, &Details.Capabilities);

	uint32_t FormatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, _VulkanSurface, &FormatCount, nullptr);

	if (FormatCount != 0)
	{
		Details.Formats.resize(FormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, _VulkanSurface, &FormatCount, Details.Formats.data());
	}

	uint32_t PresentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, _VulkanSurface, &PresentModeCount, nullptr);

	if (PresentModeCount != 0)
	{
		Details.PresentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, _VulkanSurface, &PresentModeCount, Details.PresentModes.data());
	}

	return Details;
}

void OEVulkan::Swapchain::CreateSwapchain(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler)
{
	SwapChainSupportDetails SwapChainSupport = QuerySwapChainSupport(OpaqueEngineVulkanHandler.VulkanPhysicalDevice, OpaqueEngineVulkanHandler.VulkanSurfaceKHR);

	VkSurfaceFormatKHR SurfaceFormat = ChooseSwapSurfaceFormat(SwapChainSupport.Formats);
	VkPresentModeKHR PresentMode = ChooseSwapPresentMode(SwapChainSupport.PresentModes);
	VkExtent2D Extent = ChooseSwapExtent(SwapChainSupport.Capabilities, OpaqueEngineVulkanHandler);

	uint32_t ImageCount = SwapChainSupport.Capabilities.minImageCount + 1;

	if (SwapChainSupport.Capabilities.maxImageCount > 0 && ImageCount > SwapChainSupport.Capabilities.maxImageCount)
		ImageCount = SwapChainSupport.Capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR SwapchainCreateInfo{};
	SwapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapchainCreateInfo.surface = OpaqueEngineVulkanHandler.VulkanSurfaceKHR;
	SwapchainCreateInfo.minImageCount = ImageCount;
	SwapchainCreateInfo.imageFormat = SurfaceFormat.format;
	SwapchainCreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
	SwapchainCreateInfo.imageExtent = Extent;
	SwapchainCreateInfo.imageArrayLayers = 1;
	SwapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	OEVulkan::Device::VulkanQueueFamilyIndices QueueIndices = OEVulkan::Device::FindQueueFamilies(OpaqueEngineVulkanHandler.VulkanPhysicalDevice, OpaqueEngineVulkanHandler.VulkanSurfaceKHR);
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
	if (vkCreateSwapchainKHR(OpaqueEngineVulkanHandler.VulkanDevice, &SwapchainCreateInfo, nullptr, &OpaqueEngineVulkanHandler.VulkanSwapchain) != VK_SUCCESS)
		throw std::runtime_error("Failed to create swap chain!");

	vkGetSwapchainImagesKHR(OpaqueEngineVulkanHandler.VulkanDevice, OpaqueEngineVulkanHandler.VulkanSwapchain, &ImageCount, nullptr);
	OpaqueEngineVulkanHandler.SwapchainImages.reserve(ImageCount);
	vkGetSwapchainImagesKHR(OpaqueEngineVulkanHandler.VulkanDevice, OpaqueEngineVulkanHandler.VulkanSwapchain, &ImageCount, OpaqueEngineVulkanHandler.SwapchainImages.data());

	OpaqueEngineVulkanHandler.VulkanSwapchainImageFormat = SurfaceFormat.format;
	OpaqueEngineVulkanHandler.VulkanSwapchainExtent = Extent;
}

VkExtent2D OEVulkan::Swapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities, OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler)
{
	if (Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return Capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(OpaqueEngineVulkanHandler.pApplicationWindow, &width, &height);

		VkExtent2D ActualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		ActualExtent.width = std::clamp(ActualExtent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
		ActualExtent.height = std::clamp(ActualExtent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);

		return ActualExtent;
	}
}

VkSurfaceFormatKHR OEVulkan::Swapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats)
{
	for (const auto& AvailableFormat : AvailableFormats)
	{
		if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && AvailableFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) // Preffered
			return AvailableFormat;
	}

	return AvailableFormats[0]; // If the preffered one isnt here for us we just return the first option, so if you are having issues related to wierd color space and color values look here.
}

VkPresentModeKHR OEVulkan::Swapchain::ChooseSwapPresentMode(std::vector<VkPresentModeKHR>& AvailablePresentModes)
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









/* Graphics Pipeline */

void OEVulkan::GraphicsPipeline::CreateGraphicsPipeline(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler)
{
	/* SHADER STAGE */
	std::vector<VkPipelineShaderStageCreateInfo> ShaderStagesCreateInfo = OEVulkan::GraphicsPipeline::CreateShaderStage(OpaqueEngineVulkanHandler.VulkanDevice);
	
	/* VERTEX INPUT */ 
	VkPipelineVertexInputStateCreateInfo VertexInputCreateInfo = OEVulkan::GraphicsPipeline::CreateVertexInputState();
	
	/* INPUT ASSEMBLY */ 
	VkPipelineInputAssemblyStateCreateInfo InputAssemblyCreateInfo = OEVulkan::GraphicsPipeline::CreateInputAssemblyState();
	
	/* VIEWPORT AND SCISSORS */
	VkViewport VulkanViewport = OEVulkan::GraphicsPipeline::CreateViewport(OpaqueEngineVulkanHandler.VulkanSwapchainExtent);
	VkRect2D VulkanScissor = OEVulkan::GraphicsPipeline::CreateScissor(OpaqueEngineVulkanHandler.VulkanSwapchainExtent);

	VkPipelineViewportStateCreateInfo ViewportStateCreateInfo{};
	ViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

	ViewportStateCreateInfo.viewportCount = 1;
	ViewportStateCreateInfo.pViewports = &VulkanViewport;

	ViewportStateCreateInfo.scissorCount = 1;
	ViewportStateCreateInfo.pScissors = &VulkanScissor;

	/* DYNAMIC STATES */
	std::vector<VkDynamicState> DynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo DynamicStateCreateInfo{};
	DynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	DynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(DynamicStates.size());
	DynamicStateCreateInfo.pDynamicStates = DynamicStates.data();

	/* Rasterizer */
	VkPipelineRasterizationStateCreateInfo RasterizerCreateInfo = OEVulkan::GraphicsPipeline::CreateRasterizationState();

	/* Multisampling */
	VkPipelineMultisampleStateCreateInfo MultisamplingCreateInfo = OEVulkan::GraphicsPipeline::CreateMultisampleState();

	/* Depth and stencil testing */

	/* Color blending */
	VkPipelineColorBlendAttachmentState ColorBlendAttachmentState = OEVulkan::GraphicsPipeline::CreateColorBlendAttachmentState();

}

std::vector<VkPipelineShaderStageCreateInfo> OEVulkan::GraphicsPipeline::CreateShaderStage(VkDevice _VulkanDevice)
{
	std::vector<std::string> ShaderFilePaths = {
		"E:\\Projects\\.vulkan\\Opaque\\Opaque\\Source\\Shaders\\Compiled\\DefaultShaderFragment.spv",  // Where they are located to me, persitent data paths is not a thing yet
		"E:\\Projects\\.vulkan\\Opaque\\Opaque\\Source\\Shaders\\Compiled\\DefaultShaderVertex.spv",  // Where they are located to me, persitent data paths is not a thing yet
	};

	std::vector<char> VertexShaderCodeBuffer = OEVulkan::GraphicsPipeline::ShaderManager::ReadShaderFile(ShaderFilePaths[0]);
	std::vector<char> FragmentShaderCodeBuffer = OEVulkan::GraphicsPipeline::ShaderManager::ReadShaderFile(ShaderFilePaths[1]);

	VkShaderModule VertexShaderModule = OEVulkan::GraphicsPipeline::ShaderManager::CreateShaderModule(VertexShaderCodeBuffer, _VulkanDevice);
	VkShaderModule FragmentShaderModule = OEVulkan::GraphicsPipeline::ShaderManager::CreateShaderModule(FragmentShaderCodeBuffer, _VulkanDevice);

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

	std::vector<VkPipelineShaderStageCreateInfo> ShaderStagesCreateInfo = { VertexShaderStageInfo, FragmentShaderStageInfo };

	vkDestroyShaderModule(_VulkanDevice, VertexShaderModule, nullptr);
	vkDestroyShaderModule(_VulkanDevice, FragmentShaderModule, nullptr);

	return ShaderStagesCreateInfo;
}

VkPipelineVertexInputStateCreateInfo OEVulkan::GraphicsPipeline::CreateVertexInputState()
{
	VkPipelineVertexInputStateCreateInfo NewVkPipelineVertexInputStateCreateInfo{};
	NewVkPipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	NewVkPipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
	NewVkPipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = nullptr;
	NewVkPipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;
	NewVkPipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = nullptr;

	return NewVkPipelineVertexInputStateCreateInfo;
}

VkPipelineInputAssemblyStateCreateInfo OEVulkan::GraphicsPipeline::CreateInputAssemblyState()
{
	VkPipelineInputAssemblyStateCreateInfo InputAssemblyCreateInfo{};
	InputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	InputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	InputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

	return InputAssemblyCreateInfo;
}

VkViewport OEVulkan::GraphicsPipeline::CreateViewport(VkExtent2D _VulkanSwapchainExtent)
{
	VkViewport VulkanViewport{};
	VulkanViewport.x = 0.0f;
	VulkanViewport.y = 0.0f;
	VulkanViewport.width = (float)_VulkanSwapchainExtent.width;
	VulkanViewport.height = (float)_VulkanSwapchainExtent.height;
	VulkanViewport.minDepth = 0.0f;
	VulkanViewport.maxDepth = 1.0f;

	return VulkanViewport;
}

VkRect2D OEVulkan::GraphicsPipeline::CreateScissor(VkExtent2D _VulkanSwapchainExtent)
{
	VkRect2D VulkanScissor{};
	VulkanScissor.offset = { 0, 0 };
	VulkanScissor.extent = _VulkanSwapchainExtent;

	return VulkanScissor;
}

VkPipelineRasterizationStateCreateInfo OEVulkan::GraphicsPipeline::CreateRasterizationState()
{
	VkPipelineRasterizationStateCreateInfo RasterizerCreateInfo{};
	RasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	RasterizerCreateInfo.depthClampEnable = VK_FALSE;
	RasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	RasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	RasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	RasterizerCreateInfo.lineWidth = 1.0f;
	RasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;;
	RasterizerCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	RasterizerCreateInfo.depthBiasEnable = VK_FALSE;

	return RasterizerCreateInfo;
}

VkPipelineMultisampleStateCreateInfo OEVulkan::GraphicsPipeline::CreateMultisampleState()
{
	VkPipelineMultisampleStateCreateInfo MultisamplingCreateInfo{};
	MultisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	MultisamplingCreateInfo.sampleShadingEnable = VK_FALSE;
	MultisamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	MultisamplingCreateInfo.minSampleShading = 1.0f;
	MultisamplingCreateInfo.pSampleMask = nullptr;
	MultisamplingCreateInfo.alphaToCoverageEnable = VK_FALSE;
	MultisamplingCreateInfo.alphaToOneEnable = VK_FALSE;

	return MultisamplingCreateInfo;
}

VkPipelineColorBlendAttachmentState OEVulkan::GraphicsPipeline::CreateColorBlendAttachmentState()
{
	VkPipelineColorBlendAttachmentState ColorBlendAttachmentState{};
	ColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	ColorBlendAttachmentState.blendEnable = VK_TRUE;
	ColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	ColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	ColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	ColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	ColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	ColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

	return ColorBlendAttachmentState;
}

					/* Shader Manager */

std::vector<char> OEVulkan::GraphicsPipeline::ShaderManager::ReadShaderFile(const std::string& CompiledSPVShaderFilePath)
{
	// Reads the file
	std::ifstream ShaderFile(CompiledSPVShaderFilePath, std::ios::ate | std::ios::binary);
	if (!ShaderFile.is_open()) { throw std::runtime_error("Failed to open " + CompiledSPVShaderFilePath + "!\n"); }

	// Deals with the data
	size_t ShaderFileSize = (size_t)ShaderFile.tellg();
	std::vector<char> ShaderFileBuffer(ShaderFileSize);

	ShaderFile.seekg(0);
	ShaderFile.read(ShaderFileBuffer.data(), ShaderFileSize);

	//Finished
	ShaderFile.close();

	// Debugging couts which are safe to remove
	std::cout << "The file " << CompiledSPVShaderFilePath << "'s buffer is " << ShaderFileBuffer.size() << " bytes in size" << std::endl;

	return ShaderFileBuffer;
}

VkShaderModule OEVulkan::GraphicsPipeline::ShaderManager::CreateShaderModule(const std::vector<char>& ShaderCode, VkDevice _VulkanDevice)
{
	VkShaderModuleCreateInfo VulkanShaderModuleCreateInfo{};
	VulkanShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	VulkanShaderModuleCreateInfo.codeSize = ShaderCode.size();
	VulkanShaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(ShaderCode.data());

	VkShaderModule ShaderModule;
	if (vkCreateShaderModule(_VulkanDevice, &VulkanShaderModuleCreateInfo, nullptr, &ShaderModule) != VK_SUCCESS)
		throw std::runtime_error("Failed to create a shader module!");

	return ShaderModule;
}