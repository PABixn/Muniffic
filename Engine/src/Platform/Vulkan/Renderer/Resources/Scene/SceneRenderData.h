#pragma once
#include "entt.hpp"
#include "glm.hpp"
#include <vector>
#include "Platform/Vulkan/Renderer/Resources/Buffer/VertexBuffer.h"
#include "Platform/Vulkan/Renderer/Resources/Buffer/IndexBuffer.h"
#include "Platform/Vulkan/Renderer/Resources/Buffer/SSBuffer.h"
#include "Platform/Vulkan/Renderer/Resources/Vertex/Vertex.h"
#include "Platform/Vulkan/Renderer/Resources/Descriptors/MatrixBufferDescriptorHelper.h"
#include "ObjectRenderData.h"
namespace eg {
	struct VulkanVertexBuffer;

	class SceneRenderData
	{
		std::vector<ObjectRenderData*> m_RenderDataThatNeedsUpdateThisFrame;
	public:
		bool m_Loaded = false;
		SceneRenderData() : m_RenderDataThatNeedsUpdateThisFrame() {  };
		~SceneRenderData() {  };
		void loadScene(const entt::registry& entityRegistry);
		void unloadScene();
		void createBuffers(size_t verticesPerObject, size_t numberOfObjects = 10);
		ObjectRenderData addSquare();
		void AddObjectToBuffers(ObjectRenderData& objRenderData, void* verticesData, void* indicesData, void* matricesData = nullptr);
		MatrixBufferDescriptorHelper& getMatrixBufferDescriptorHelper(){ return m_DescriptorHelper; }
		VulkanVertexBuffer m_VertexBuffer;
		VulkanIndexBuffer m_IndexBuffer;
		VulkanShaderStorageBuffer m_SSBO;
		MatrixBufferDescriptorHelper m_DescriptorHelper;
	};
}