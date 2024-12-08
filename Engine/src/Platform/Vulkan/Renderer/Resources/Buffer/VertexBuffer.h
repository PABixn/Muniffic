#pragma once
#include "Platform/Vulkan/Renderer/Resources/Buffer.h"
namespace eg {
	struct ObjectRenderData;
	enum VertexType {
		VertexType_Undefined = 0,
		VertexType_BasicMesh //	See struct: VulkanBasicMeshVertex			Used for vertices of basic objects: rects, cubes
	};
	struct VulkanVertexBuffer {
	public:
		VulkanBuffer m_Buffer;
		void* m_Mapped;
		size_t m_VerticesCount = 0;
		size_t m_LastOffset = 0;
		VertexType m_VertexType;
		bool m_LeftSpace;
		bool addBasic2DObjectVertices(ObjectRenderData* objectToAdd, void* verticesData);
		void create(size_t size, VertexType vertexType);
		void destroy();
	};
}