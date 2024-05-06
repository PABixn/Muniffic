#include "egpch.h"
#include "VulkanVertex.h"
#include "VulkanBufferFactory.h"
#include "VulkanDeviceMemoryManager.h"
namespace eg {

	VkVertexInputBindingDescription& VulkanVertexInputLayout::GetBindingDescription(){
		{
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = GetStride();
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}
	}

	std::vector<VkVertexInputAttributeDescription>& VulkanVertexInputLayout::GetAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
		uint32_t offset = 0;
		for (const auto& element : GetElements()) {
			VkVertexInputAttributeDescription attributeDescription = {};
			attributeDescription.binding = 0;
			attributeDescription.location = offset;
			attributeDescription.format = ShaderDataTypeToVulkanFormat(element.Type);
			attributeDescription.offset = element.Offset;
			attributeDescriptions.push_back(attributeDescription);
			offset++;
		}
		return attributeDescriptions;
	}

	VulkanVertexBuffer::VulkanVertexBuffer(const VulkanBuffer& buffer)
: m_VertexBuffer(buffer)
	{
	}

	void VulkanVertexBuffer::Cleanup(VkDevice logicalDevice)
	{
		m_VertexBuffer.Destroy(logicalDevice);
	}

	void VulkanVertexBuffer::Bind() const
	{
		VkBuffer buffers[] = { m_VertexBuffer.GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void VulkanVertexBuffer::SetData(void* data, uint32_t size)
	{
		VulkanDeviceMemoryManager::MapMemory(m_VertexBuffer.GetBufferMemory(), size, (float*)data);
	}

	Ref<VertexBuffer> VulkanVertexBuffer::Create(uint32_t size)
	{
		return CreateRef<VertexBuffer>(VulkanBufferFactory::GetInstance()->CreateVertexBuffer(size));
	}

	Ref<VertexBuffer> VulkanVertexBuffer::Create(float* vertices, uint32_t size)
	{
		return CreateRef<VertexBuffer>(VulkanBufferFactory::GetInstance()->CreateVertexBuffer(vertices, size));
	}

}