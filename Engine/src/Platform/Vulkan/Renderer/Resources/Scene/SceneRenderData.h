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
#include "Engine/Scene/Scene.h"
namespace eg {
	struct VulkanVertexBuffer;
	class Entity;
	class SceneRenderData
	{
	public:
		std::unordered_map<UUID, ObjectRenderData> m_EntityRenderInfoMap;
		size_t m_LastRenderData = 0;
		Scene* m_Scene;
		bool m_Loaded = false;
		SceneRenderData() {  };
		~SceneRenderData() {  };
		void loadScene(Scene* scene);
		void unloadScene();
		void createBuffers(size_t verticesPerObject, size_t numberOfObjects = 10);
		ObjectRenderData addSquare(const glm::vec3 color, const glm::mat4& transform);
		void addSquare(Entity& entity, entt::registry* registry = nullptr); // registry in case the current entity is still loading. Proper use in other scenarios is passing only the entity
		void removeSquare(Entity entity);
		void AddObjectToBuffers(ObjectRenderData& objRenderData, void* indicesData, void* matricesData);
		void UpdateSpriteRenderComponentData(Entity& entity, entt::registry* reg = nullptr);
		void UpdateMatrixData(const Entity& objRenderData);
		void UpdateAllMatrixData(Scene* scene);
		UUID findLastRenderData();
		MatrixBufferDescriptorHelper& getMatrixBufferDescriptorHelper(){ return m_DescriptorHelper; }
		VulkanVertexBuffer m_VertexBuffer;
		VulkanIndexBuffer m_IndexBuffer;
		VulkanShaderStorageBuffer m_SSBO;
		MatrixBufferDescriptorHelper m_DescriptorHelper;
	};
}