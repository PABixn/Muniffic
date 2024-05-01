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

}