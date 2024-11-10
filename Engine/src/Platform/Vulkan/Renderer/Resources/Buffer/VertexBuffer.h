#pragma once
#include "Platform/Vulkan/Renderer/Resources/Buffer.h"
namespace eg {
	struct ObjectRenderData;
	struct VulkanVertexBuffer {
	public:
		VulkanBuffer m_Buffer;
		void* m_Mapped;
		size_t m_VerticesCount = 0;
		size_t m_LastOffset = 0;
		bool addBasic2DObjectVertices(ObjectRenderData* objectToAdd, void* verticesData);
		void create(size_t size);
		void destroy();
	};
}