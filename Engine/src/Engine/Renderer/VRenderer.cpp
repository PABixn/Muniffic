#include "C:/Users/Natal/Documents/muniffic/Muniffic/Engine/CMakeFiles/Engine.dir/Debug/cmake_pch.hxx"
#include "VRenderer.h"
#include "Platform/Vulkan/VulkanRenderer.h"
eg::VulkanRenderer VRen;

void eg::VRenderer::Init()
{
	VRen.init();
}

void eg::VRenderer::Shutdown()
{
	VRen.cleanUp();
}
