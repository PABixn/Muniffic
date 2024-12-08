#pragma once
#include "Platform/Vulkan/Renderer/Resources/Buffer.h"
namespace eg {
	struct ObjectRenderData;
	struct alignas(16) ObjectMatrixData {
		glm::vec3 color;
		uint32_t padding;
		glm::mat4 model;
	};
	class VulkanShaderStorageBuffer
	{
	public:
		VulkanBuffer m_Buffer;
		void* m_Mapped;
		size_t m_LastOffset = 0;
		size_t m_InstancesCount = 0;
		bool addBasic2DObjectSRComponentData(ObjectRenderData* objectToAdd, void* indicesData);
		void create(size_t size);
		void destroy();
	private:

	};
}