#include "VRenderer.h"
#include "Platform/Vulkan/VulkanRenderer.h"
void eg::VRenderer::Init()
{
	VRen::get().init();
}

void eg::VRenderer::Shutdown()
{
	VRen::get().cleanUp();
}
