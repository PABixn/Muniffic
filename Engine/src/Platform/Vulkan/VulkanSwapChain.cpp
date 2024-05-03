#include "egpch.h"
#include "VulkanSwapChain.h"
#include "VulkanCommandManager.h"
#include "Engine/Core/Application.h"
#include "VulkanContext.h"

namespace eg {

    VulkanHandler* getVulkanHandler() {
		return &((VulkanContext*)(Application::Get().GetWindow().GetContext()))->GetVulkanHandler();
    }

    VulkanSwapChainSupportDetails VulkanSwapChainSupportDetails::QuerySwapChainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
    {
        VulkanSwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);
        
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
        
        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
        }
        
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
        
        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
        }
        
        return details;
    }

    VkSurfaceFormatKHR VulkanSwapChainSupportDetails::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapChainSupportDetails::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    void VulkanSwapChain::Init()
    {
        VulkanHandler* handler = getVulkanHandler();
        VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
        VkSurfaceKHR surface = handler->GetVulkanSurface().GetSurface();
        VkPhysicalDevice physicalDevice = handler->GetVulkanPhysicalDevice().GetPhysicalDevice();
        VulkanSwapChainSupportDetails swapChainSupport = VulkanSwapChainSupportDetails::QuerySwapChainSupportDetails(physicalDevice, surface);

		VkSurfaceFormatKHR surfaceFormat = swapChainSupport.ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = swapChainSupport.ChooseSwapPresentMode(swapChainSupport.presentModes);

		VkSurfaceCapabilitiesKHR capabilities = swapChainSupport.capabilities;
        ChooseSwapExtent(capabilities);

		uint32_t imageCount = capabilities.minImageCount + 1;

		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
			imageCount = capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_Extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        VulkanQueueFamilyIndices indices = VulkanQueueFamilyIndices::FindQueueFamilies(physicalDevice, surface);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
		if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_Swapchain) != VK_SUCCESS) {
			EG_CORE_ERROR("Failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(device, m_Swapchain, &imageCount, nullptr);
		m_Images.resize(imageCount);
		vkGetSwapchainImagesKHR(device, m_Swapchain, &imageCount, m_Images.data());

		m_ImageFormat = surfaceFormat.format;
    }

    void VulkanSwapChain::CreateImageViews()
    {
        VulkanHandler* handler = getVulkanHandler();
        VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
        m_ImageViews.resize(m_Images.size());

        for (size_t i = 0; i < m_Images.size(); i++) {
            m_ImageViews[i] = VulkanImageView::Create(device, m_Images[i], m_ImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, m_MipLevels);
        }
    }

    void VulkanSwapChain::CreateColorResources()
    {
        VulkanHandler* handler = getVulkanHandler();
        VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
        m_ColorImage = VulkanImage::Create(device, m_Extent.width, m_Extent.height, 1, m_MsaaSamples, m_ImageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    void VulkanSwapChain::CreateDepthResources()
    {
        VulkanHandler* handler = getVulkanHandler();
        VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
        VkFormat depthFormat = VulkanImage::FindDepthFormat(device);
        m_DepthImage = VulkanImage::Create(device, m_Extent.width, m_Extent.height, 1, m_MsaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);
    }

    void VulkanSwapChain::CreateFramebuffers()
    {
        VulkanHandler* handler = getVulkanHandler();
        VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
        VkRenderPass renderPass = handler->GetVulkanRenderPass().GetRenderPass();
        m_Framebuffers.resize(m_ImageViews.size());
        for (size_t i = 0; i < m_ImageViews.size(); i++)
        {
            std::array<VkImageView, 3> attachments = {
                m_ColorImage.GetVulkanImageView().GetImageView(),
                m_DepthImage.GetVulkanImageView().GetImageView(),
                m_ImageViews[i].GetImageView()
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = m_Extent.width;
            framebufferInfo.height = m_Extent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_Framebuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    void VulkanSwapChain::RecreateSwapChain()
    {
        VulkanHandler* handler = getVulkanHandler();
        VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
        int width = 0, height = 0;
        glfwGetFramebufferSize(handler->GetWindow(), &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(handler->GetWindow(), &width, &height);
            glfwWaitEvents();
        }

        //frameBufferResized = false;

        vkDeviceWaitIdle(device);

        CleanupSwapChain();

        Init();
        CreateImageViews();
        CreateColorResources();
        CreateDepthResources();
        CreateFramebuffers();
    }

    void VulkanSwapChain::generateMipMaps(VkImage image, VkFormat format, int32_t texWidth, int32_t texHeight, int32_t mipLevels)
    {
        VulkanHandler* handler = getVulkanHandler();
        VkPhysicalDevice physicalDevice = handler->GetVulkanPhysicalDevice().GetPhysicalDevice();
        VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
        VkCommandPool commandPool = handler->GetVulkanCommandPool().GetPool();
        VkQueue graphicsQueue = handler->GetVulkanLogicalDevice().GetGraphicsQueue();

        VkFormatProperties formatProperties{};

        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

        if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
            throw std::runtime_error("texture image format does not support linear blitting");
        }

       VulkanCommandBuffer commandBuffer = VulkanCommandManager::BeginSingleTimeCommands();

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.layerCount = 1;



        int32_t mipWidth = texWidth;
        int32_t mipHeight = texHeight;

        for (uint32_t i = 1; i < mipLevels; i++) {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = { 0,0,0 };
            blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = { 0,0,0 };
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer,
                image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &blit,
                VK_FILTER_LINEAR);

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        VulkanCommandManager::EndSingleTimeCommands(commandBuffer);
    }

    VkSampleCountFlagBits VulkanSwapChain::GetMaxUsableSampleCount()
    {
        VkPhysicalDeviceProperties physicalDeviceProperties{};
        VulkanHandler* handler = getVulkanHandler();
        VkPhysicalDevice physicalDevice = handler->GetVulkanPhysicalDevice().GetPhysicalDevice();
        vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

        VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
    }

    void VulkanSwapChain::Cleanup()
    {
        VulkanHandler* handler = getVulkanHandler();
		VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
        vkDestroySwapchainKHR(device, m_Swapchain, nullptr);
    }

    void VulkanSwapChain::CleanupSwapChain()
    {
        VulkanHandler* handler = getVulkanHandler();
        VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
        m_ColorImage.Cleanup(device);

		m_DepthImage.Cleanup(device);

		for (size_t i = 0; i < m_Framebuffers.size(); i++) {
			vkDestroyFramebuffer(device, m_Framebuffers[i], nullptr);
		}

		for (size_t i = 0; i < m_ImageViews.size(); i++) {
			m_ImageViews[i].Cleanup(device);
		}

		vkDestroySwapchainKHR(device, m_Swapchain, nullptr);
    }

    void VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != UINT32_MAX) {
			m_Extent = capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(getVulkanHandler()->GetWindow(), &width, &height);

            m_Extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

            m_Extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, m_Extent.width));
            m_Extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, m_Extent.height));
		}
    }

}