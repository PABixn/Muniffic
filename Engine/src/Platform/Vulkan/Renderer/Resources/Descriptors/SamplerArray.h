#pragma once
#include "Platform/Vulkan/Renderer/Resources/Buffer.h"
namespace eg {
	struct ObjectRenderData;
	class VulkanSamplerArray
	{
	public:
		VulkanBuffer m_Buffer;
		void* m_Mapped;
		size_t m_LastOffset = 0;
		size_t m_SamplerCount = 0;
		uint32_t addSampler(VkSampler* sampler);
		void create(size_t size);
		void destroy();
	private:

	};
}