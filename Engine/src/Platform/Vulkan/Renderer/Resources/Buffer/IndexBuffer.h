#pragma once
#include "Platform/Vulkan/Renderer/Resources/Buffer.h"
namespace eg{
	struct ObjectRenderData;
	struct VulkanIndexBuffer
	{
	public:
		VulkanBuffer m_Buffer;
		void* m_Mapped;
		size_t m_IndicesCount = 0;
		size_t m_LastOffset = 0;
		bool addBasic2DObjectIndices(ObjectRenderData* objectToAdd, void* indicesData);
		void create(size_t size);
		void destroy();
	};
}