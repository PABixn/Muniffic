#pragma once
#include <vector>
#include "Vertex/Vertex.h"
#include "Descriptors/DescriptorsManager.h"
#include "Frame/FrameManager.h"
#include "Images/TextureManager.h"
#include "Buffer.h"
#include <glm/mat4x4.hpp>
#include "Buffer/VertexBuffer.h"
namespace eg {
	extern std::vector<VulkanVertex> Vertices;
	extern std::vector<uint16_t> Indices;
	
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	class ResourceManager
	{
	public:
		// General usage
		VulkanBuffer createBuffer(const VkDeviceSize& size, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& properties);
		void DestroyBuffer(VulkanBuffer& buffer);
		void LoadModel(const char* path);

		void copyDataToBuffer(VulkanBuffer& buffer, void* data);
		void copyBuffer(VulkanBuffer& srcBuffer, VulkanBuffer& destBuffer, VkDeviceSize size);
		std::vector<VkBuffer> getVertexBuffers() { return { m_VertexBuffer.m_Buffer.m_Buffer }; };
		VkBuffer getIndexBuffer() { return m_IndexBuffer.m_Buffer; };
		size_t getVerticesCount() { return m_VertexBuffer.m_VerticesCount; }
		size_t getIndicesCount() { return  m_IndicesCount; }
		VkDescriptorSetLayout& getDescriptorSetLayout() { return m_DescriptorManager.getDescriptorSetLayout(); }
		std::vector<VkDescriptorSet>& getDescriptorSets() { return m_DescriptorManager.m_DescriptorSets; }
		TextureManager& getTextureManager() { return m_TextureManager; }
	public:
		void init();
		void cleanUp();
	public:
		bool m_ModelLoaded = false;
		size_t m_IndicesCount = 0;

		// Managers
		DescriptorsManager m_DescriptorManager;
		FrameManager m_FrameManager;
		TextureManager m_TextureManager;
	private:
		VulkanVertexBuffer m_VertexBuffer;
		VulkanBuffer m_IndexBuffer;
	private:
	};
}