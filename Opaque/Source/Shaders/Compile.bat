@echo off

:: Compilation
echo Compiling vertex shaders...
C:\VulkanSDK\1.3.239.0\Bin\glslc.exe "E:\Projects\.vulkan\Opaque\Opaque\Source\Shaders\DefaultShaderFragment.frag" -o "E:\Projects\.vulkan\Opaque\Opaque\Source\Shaders\Compiled\DefaultShaderFragment.spv"

echo Compiling fragment shaders...
C:\VulkanSDK\1.3.239.0\Bin\glslc.exe "E:\Projects\.vulkan\Opaque\Opaque\Source\Shaders\DefaultShaderVertex.vert" -o "E:\Projects\.vulkan\Opaque\Opaque\Source\Shaders\Compiled\DefaultShaderVertex.spv"

echo Finished compilation of shaders

pause