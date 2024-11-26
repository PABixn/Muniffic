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
	struct FreeObjectRenderData {
		size_t sizeOfVerticesPerObject = 0;
		size_t sizeOfIndicesPerObject = 0;
		size_t sizeOfMatrixDataPerObject = 0;
		std::vector<ObjectRenderData> freeBlocks;
	};
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
		void addSquare(Entity& entity, entt::registry* registry = nullptr); // SRcomponent in case the current entity is still loading and the onComponentAdd calls this before adding the SpriteRendererComponent and TransformComponent to Scene registry. Proper use in other scenarios is passing only the entity
		void removeSquare(Entity entity);
		void AddObjectToBuffers(ObjectRenderData& objRenderData, void* verticesData, void* indicesData, void* matricesData = nullptr);
		void UpdateSpriteRenderComponentData(Entity& entity, entt::registry* reg = nullptr);
		void UpdateMatrixData(Entity& objRenderData);
		MatrixBufferDescriptorHelper& getMatrixBufferDescriptorHelper(){ return m_DescriptorHelper; }
		VulkanVertexBuffer m_VertexBuffer;
		VulkanIndexBuffer m_IndexBuffer;
		VulkanShaderStorageBuffer m_SSBO;
		MatrixBufferDescriptorHelper m_DescriptorHelper;
		FreeObjectRenderData m_FreeObjectRenderDataBlocks;
	};
}