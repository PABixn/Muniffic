#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanBuffer.h"
#include "Engine/Renderer/Buffer.h"
namespace eg {
	class VulkanIndexBuffer : public IndexBuffer{
	public:
		//Might turn this into a template class later for different index types
		VulkanIndexBuffer() = default;
		VulkanIndexBuffer(const VulkanBuffer& buffer, uint32_t indexCount) : m_Indexbuffer(buffer), indexCount(indexCount) {}
		VulkanIndexBuffer(const VulkanIndexBuffer& other) = default;
		~VulkanIndexBuffer() = default;

		static Ref<VulkanIndexBuffer> Create(uint32_t* vertices, uint32_t count);
		virtual void AddData(uint32_t* data, uint32_t size) override;
		virtual void SetData(void* data, uint32_t size) override;
		virtual void SetData() override;
		void Cleanup(VkDevice device);

		virtual void Bind() const;

		virtual uint32_t GetCount() const { return indexCount; }
		VulkanBuffer& GetBuffer() { return m_Indexbuffer; }
	private:
		VulkanBuffer m_Indexbuffer;
		std::vector<uint32_t> m_Indices;
		uint32_t indexCount;
	};
}