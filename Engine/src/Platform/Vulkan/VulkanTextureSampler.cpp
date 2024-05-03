#include "egpch.h"
#include "VulkanTextureSampler.h"
#include "VulkanUtils.h"
namespace eg {
	void VulkanTextureSampler::Create()
	{
		VulkanHandler* handler = GetVulkanHandler();
		VkDevice logicalDevice = handler->GetVulkanLogicalDevice().GetDevice();
		VkPhysicalDevice physicalDevice = handler->GetVulkanPhysicalDevice().GetPhysicalDevice();
		uint32_t mipLevels = handler->GetVulkanSwapChain().GetMipLevels();
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0;
		samplerInfo.maxLod = static_cast<float>(mipLevels);

		if (vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &m_Sampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
	}

	void VulkanTextureSampler::Cleanup()
	{
		VulkanHandler* handler = GetVulkanHandler();
		VkDevice logicalDevice = handler->GetVulkanLogicalDevice().GetDevice();
		vkDestroySampler(logicalDevice, m_Sampler, nullptr);
	}
}
