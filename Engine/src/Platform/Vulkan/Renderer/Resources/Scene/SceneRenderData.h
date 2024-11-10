#pragma once
#include "entt.hpp"
#include "glm.hpp"
#include <vector>
#include "Platform/Vulkan/Renderer/Resources/Buffer/VertexBuffer.h"
#include "Platform/Vulkan/Renderer/Resources/Buffer/IndexBuffer.h"
#include "Platform/Vulkan/Renderer/Resources/Vertex/Vertex.h"
namespace eg {
	struct VulkanVertexBuffer;

	enum RenderUpdate
	{
		RenderUpdate_None = 0,
		RenderUpdate_Created,
		RenderUpdate_VerticesSizeChanged,
		RenderUpdate_ValueChanged
	};
	struct ObjectRenderData {
		RenderUpdate m_Update;
		VulkanVertexBuffer* m_VertexBuffer;
		uint64_t m_VertexBufferOffset;
		uint32_t m_VerticesCount;
		VulkanIndexBuffer* m_IndexBuffer;
		uint64_t m_IndexBufferOffset;
		uint32_t m_IndicesCount;

	};



	class SceneRenderData
	{
		std::vector<ObjectRenderData*> m_RenderDataThatNeedsUpdateThisFrame;
	public:
		bool m_Loaded = false;
		SceneRenderData() : m_RenderDataThatNeedsUpdateThisFrame() {  };
		~SceneRenderData() {  };
		void loadScene(const entt::registry& entityRegistry);
		void unloadScene();
		ObjectRenderData addSquare(const glm::vec3& color);
		VulkanVertexBuffer m_VertexBuffer;
		VulkanIndexBuffer m_IndexBuffer;
	private:
		
	};
}