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

// OE stands for Opaque Engine, just like UE for Unreal Engine.
namespace OEVulkan
{
	/// <summary>
	/// Fill this stuct will all the data that it wants before 
	/// passing it to the constructor for the OEVulkanHandler 
	/// class in "Source/Application/Vulkan/OEVulkan.h"
	/// </summary>
	struct OEVulkanHandlerCreateInfo
	{
		uint32_t ApplicationWidth;
		uint32_t ApplicationHeight;
		std::string ApplicationName;

		GLFWwindow* pApplicationWindow;

		uint32_t VulkanAPIVersion;
	};

	/// <summary>
	/// This class is just a simple container for all the variables related to vulkan.
	/// </summary>
	class OEVulkanHandler
	{
	public:
		/* Default class functions */

		OEVulkanHandler();
		OEVulkanHandler(OEVulkanHandlerCreateInfo CreateInfo);

		//void OEVulkanInitialize();

		//void OEVulkanStart();

		//void OEVulkanUpdate();

		/// <summary>
		/// Gets directly called from Application::Cleaup();
		/// </summary>
		void OEVulkanCleanup();

		/* Vulkan Configurations and Application Settings */

		uint32_t ApplicationWidth;

		uint32_t ApplicationHeight;

		std::string ApplicationName;

		GLFWwindow* pApplicationWindow;

		uint32_t VulkanAPIVersion;

		/* Vulkan Generals */

		VkInstance VulkanInstance;

		VkDevice VulkanDevice;

		VkPhysicalDevice VulkanPhysicalDevice;

		VkSurfaceKHR VulkanSurfaceKHR;

		/* Vulkan Queues */

		VkQueue GraphicsQueue;

		VkQueue PresentQueue;

		/* Validation Layers */

		#ifdef NDEBUG
			/// <summary>
			/// Wether to enable validation layers or not. This value automatically
			/// changes based on build type(DEBUG and RELEASE)
			/// </summary>
			const bool EnableValidationLayers = false;
		#else
			/// <summary>
			/// Wether to enable validation layers or not. This value automatically
			/// changes based on build type(DEBUG and RELEASE)
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

		/* Swapchain */

		const std::vector<const char*> DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		std::vector<VkImage> SwapchainImages;

		std::vector<VkImageView> SwapchainImageViews;

		VkSwapchainKHR VulkanSwapchain;

		VkFormat VulkanSwapchainImageFormat;

		VkExtent2D VulkanSwapchainExtent;

	private:

	};

	namespace General
	{
		/// <summary>
		/// Use this function for when you want to initialize the vulkan graphics API. 
		/// </summary>
		/// <param name="OpaqueEngineVulkanHandler"/>
		/// <param name="OpaqueEngineVulkanHandlerCreateInfo"/>
		void InitializeVulkan
		(
			OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler, 
			OEVulkan::OEVulkanHandlerCreateInfo OpaqueEngineVulkanHandlerCreateInfo
		);

		/// <summary>
		/// In this function the vulkan instance gets created. I do not believe you 
		/// should ever need to call this outside of where its already called, but 
		/// id recommend you taking a look through the code to understand it.
		/// </summary>
		/// <param name="OpaqueEngineVulkanHandler"/>
		void CreateInstance(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler);

		/// <summary>
		/// Creates and then sets the VulkanSurface variable in the OEVulkanHandler which we input.
		/// </summary>
		void CreateSurface(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler);
	};

	namespace ValidationLayers
	{
		/// <summary>
		/// Checks validaion layer support. Validation layers are used for 
		/// different things in debugging. We are using this because vulkan
		/// sucks at being debugging friendly.
		/// </summary>
		/// <param name="OpaqueEngineVulkanHandler"/>
		bool CheckValidationLayerSupport(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler);
	}

	namespace Device
	{
		/// <summary>
		/// List over all the required queue indicies required for a GPU to pass the isDeviceSuitable exam
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
		/// This is the function that chooses which physical device to use. 
		/// </summary>
		/// <param name="OpaqueEngineVulkanHandler"/>
		void PickPhysicalDevice(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler);

		/// <summary>
		/// Out of the physical device we made we are going to create a vulkan device, also known as a logical device.
		/// </summary>
		void CreateLogicalDevice(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler);

		/// <summary>
		/// Checks if an input device is compatible for this application.
		/// </summary>
		bool isDeviceSuitable(VkPhysicalDevice _Device, OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler);

		bool CheckDeviceExtensionSupport(VkPhysicalDevice _Device, OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler);

		/// <summary>
		/// Find queue families
		/// </summary>
		/// <param name="_Device"></param>
		/// <returns></returns>
		OEVulkan::Device::VulkanQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _Device, VkSurfaceKHR& _VulkanSurfaceKHR);
	};

	namespace Swapchain
	{
		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR Capabilities;
			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;
		};

		/// <summary>
		/// Creates the swap chain.
		/// </summary>
		void CreateSwapchain(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler);

		/// <summary>
		/// Populates a struct with the swap chain details to check for support then returns it.
		/// </summary>
		OEVulkan::Swapchain::SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice _Device, VkSurfaceKHR& _VulkanSurface);

		/// <summary>
		/// This function chooses the swap extent.
		/// </summary>
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities, OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler);

		/// <summary>
		/// This function chooses the swap surface format.
		/// </summary>
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats);
		
		/// <summary>
		/// This function chooses the swap present mode.
		/// </summary>
		VkPresentModeKHR ChooseSwapPresentMode(std::vector<VkPresentModeKHR>& AvailablePresentModes);

		/// <summary>
		/// Creates image views
		/// </summary>
		/// <param name="OpaqueEngineVulkanHandler"></param>
		void CreateImageViews(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler);
	};

	namespace GraphicsPipeline
	{
		void CreateGraphicsPipeline(OEVulkan::OEVulkanHandler& OpaqueEngineVulkanHandler);

		/* Stages within */

		std::vector<VkPipelineShaderStageCreateInfo> CreateShaderStage(VkDevice _VulkanDevice);

		VkPipelineVertexInputStateCreateInfo CreateVertexInputState();

		VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyState();

		VkViewport CreateViewport(VkExtent2D _VulkanSwapchainExtent);

		VkRect2D CreateScissor(VkExtent2D _VulkanSwapchainExtent);

		VkPipelineRasterizationStateCreateInfo CreateRasterizationState();

		VkPipelineMultisampleStateCreateInfo CreateMultisampleState();

		VkPipelineColorBlendAttachmentState CreateColorBlendAttachmentState();

		namespace ShaderManager
		{
			std::vector<char> ReadShaderFile(const std::string& CompiledSPVShaderFilePath);

			VkShaderModule CreateShaderModule(const std::vector<char>& ShaderCode, VkDevice _VulkanDevice);
		}
	};
};