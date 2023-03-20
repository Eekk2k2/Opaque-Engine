#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <vulkan/vulkan.h>

namespace ShaderManager
{
	/* Loading and creating of shaders */

	/// <summary>
	/// Returns the contents of a compiled shader in an std::vector<char>().
	/// </summary>
	/// <param name="CompiledShaderPath"></param>
	std::vector<char> ReadShaderFile(const std::string& CompiledShaderPath);

	/// <summary>
	/// Returns a VkShaderModule
	/// </summary>
	/// <param name="ShaderCode"></param>
	/// <returns></returns>
	VkShaderModule VulkanCreateShaderModule(const std::vector<char>& ShaderCode, VkDevice _VulkanDevice); // TODO : MOVE THIS TO THE FUTURE VULKAN SOURCE AND HEADER FILE

	void CreateVertexShader(); // NOT IMPLEMENTED TODO

	void CreateFragmentShader();  // NOT IMPLEMENTED TODO : 

	// GEOMETRY

	// COMPUTE

	/* Class */

	/// <summary>
	/// This class is just a wrapper for some common variables. You get all the shader functions from namespace ShaderManager from ShaderManager.h.
	/// </summary>
	class Shader
	{
	public:
		std::string ShaderName = "Unnamed Shader";

		std::string VertexShaderPath = "";
		std::string FragmentShaderPath = "";
		
		std::string CompiledVertexShaderPath = "";
		std::string CompiledFragmentShaderPath = "";

		std::vector<char> VertexShaderCode;
		std::vector<char> FragmentShaderCode;

		VkShaderModule VertexShaderModule;
		VkShaderModule FragmentShaderModule;
	};

	/// <summary>
	/// Subshader for the shader class.
	/// </summary>
	class VertexShader
	{

	};

	/// <summary>
	/// Subshader for the shader class.
	/// </summary>
	class FragmentShader
	{

	};
}