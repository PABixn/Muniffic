#pragma once
#include "Platform/Vulkan/Renderer/Resources/Buffer.h"
namespace eg {
	struct ObjectRenderData;
	struct ObjectMatrixData {
		glm::vec3 color;
		glm::mat4 model;
	};
	class VulkanShaderStorageBuffer
	{
	public:
		VulkanBuffer m_Buffer;
		void* m_Mapped;
		size_t m_LastOffset = 0;
		size_t m_InstancesCount = 0;
		bool addBasic2DObjectModelMatrix(ObjectRenderData* objectToAdd, void* indicesData);
		void create(size_t size);
		void destroy();
	private:

	};
}