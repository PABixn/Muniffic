#include "egpch.h"
#include "VulkanVertex.h"

namespace eg {

	VkVertexInputBindingDescription VulkanVertexInputLayout::GetBindingDescription(){
		{
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = GetStride();
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}
	}

	std::vector<VkVertexInputAttributeDescription> VulkanVertexInputLayout::GetAttributeDescriptions() {
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

	void VulkanVertexBuffer::Create(VkDevice logicalDevice, VkPhysicalDevice physicalDevice,float* vertices, uint32_t size, uint32_t stride)
	{
		VkDeviceSize bufferSize = stride * size;

		VulkanBuffer stagingBuffer;
		stagingBuffer.CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertices);

		m_VertexBuffer.CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		m_VertexBuffer.CopyFrom(stagingBuffer, stagingBuffer.GetSize());

		stagingBuffer.Destroy(logicalDevice);
	}

}