#include "ShaderManager.h"

std::vector<char> ShaderManager::ReadShaderFile(const std::string& CompiledShaderFilePath)
{
	// Reads the file
	std::ifstream ShaderFile(CompiledShaderFilePath, std::ios::ate | std::ios::binary);
	if (!ShaderFile.is_open()) { throw std::runtime_error("Failed to open " + CompiledShaderFilePath + "!\n"); }

	// Deals with the data
	size_t ShaderFileSize = (size_t)ShaderFile.tellg();
	std::vector<char> ShaderFileBuffer(ShaderFileSize);

	ShaderFile.seekg(0);
	ShaderFile.read(ShaderFileBuffer.data(), ShaderFileSize);

	//Finished
	ShaderFile.close();

	// Debugging couts which are safe to remove
	std::cout << "The file " << CompiledShaderFilePath << "'s buffer is " << ShaderFileBuffer.size() << " bytes in size" << std::endl;

	return ShaderFileBuffer;
}

VkShaderModule ShaderManager::VulkanCreateShaderModule(const std::vector<char>& ShaderCode, VkDevice _VulkanDevice)
{
	VkShaderModuleCreateInfo VulkanShaderModuleCreateInfo{};
	VulkanShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	VulkanShaderModuleCreateInfo.codeSize = ShaderCode.size();
	VulkanShaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(ShaderCode.data());

	VkShaderModule ShaderModule;
	if (vkCreateShaderModule(_VulkanDevice, &VulkanShaderModuleCreateInfo, nullptr, &ShaderModule) != VK_SUCCESS)
		throw std::runtime_error("Failed to create a shader movule!");

	return ShaderModule;
}
