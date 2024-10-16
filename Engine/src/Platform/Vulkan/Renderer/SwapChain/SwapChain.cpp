#define NOMINMAX
#include "SwapChain.h"
#include "Renderer/Device/Device.h"
#include <iostream>
#include <algorithm>
#include "Platform/Vulkan/VulkanRenderer.h"
namespace eg {
//Helper
    SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
    {
        {

            SwapChainSupportDetails details;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

            if (formatCount != 0) {
                details.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

            if (presentModeCount != 0) {
                details.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
            }

            return details;
        }
    }


    void SwapChain::create(GLFWwindow* window)
    {
        Device& device = VRen::get().getDevice();
        VkSwapchainCreateInfoKHR SwapChainCreateInfo{};
        SwapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        SwapChainCreateInfo.surface = device.m_Surface;

        SwapChainSupportDetails supportDetails = querySwapChainSupport(device.m_PhysicalDevice, device.m_Surface);

        // Surface format
        for (const auto& availableFormat : supportDetails.formats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                m_ImageFormat = availableFormat.format;
                SwapChainCreateInfo.imageColorSpace = availableFormat.colorSpace;
                break;
            }
        }
        if (m_ImageFormat == VK_FORMAT_UNDEFINED) {
            m_ImageFormat = supportDetails.formats[0].format;
            SwapChainCreateInfo.imageColorSpace = supportDetails.formats[0].colorSpace;
        }
        SwapChainCreateInfo.imageFormat = m_ImageFormat;

        // Presentation mode
        for (const auto& availablePresentMode : supportDetails.presentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                SwapChainCreateInfo.presentMode = availablePresentMode;
                break;
            }
        }
        if (SwapChainCreateInfo.presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
        {
            SwapChainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

        }
        SwapChainCreateInfo.clipped = VK_TRUE;

        // Swap extent
        if (supportDetails.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            m_Extent = supportDetails.capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, supportDetails.capabilities.minImageExtent.width, supportDetails.capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, supportDetails.capabilities.minImageExtent.height, supportDetails.capabilities.maxImageExtent.height);

            m_Extent = actualExtent;
        }
        SwapChainCreateInfo.imageExtent = m_Extent;

        // Image count
        SwapChainCreateInfo.minImageCount = supportDetails.capabilities.minImageCount + 1;
        if (supportDetails.capabilities.maxImageCount > 0 && SwapChainCreateInfo.minImageCount > supportDetails.capabilities.maxImageCount) {
            SwapChainCreateInfo.minImageCount = supportDetails.capabilities.maxImageCount;
        }

        //Other
        SwapChainCreateInfo.imageArrayLayers = 1;
        SwapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        SwapChainCreateInfo.preTransform = supportDetails.capabilities.currentTransform;
        SwapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        SwapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        // Queue families
        QueueFamilyIndices indices = Queue::GetQueueFamiliesIndices(device.m_PhysicalDevice, device.m_Surface);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily) {
            SwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            SwapChainCreateInfo.queueFamilyIndexCount = 2;
            SwapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            SwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            SwapChainCreateInfo.queueFamilyIndexCount = 0;
            SwapChainCreateInfo.pQueueFamilyIndices = nullptr;
        }

        if (vkCreateSwapchainKHR(device.m_LogicalDevice, &SwapChainCreateInfo, nullptr, &m_NativeSwapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain! :(");
        }

        // Getting swapchain images
        uint32_t imageCount;
        vkGetSwapchainImagesKHR(device.m_LogicalDevice, m_NativeSwapChain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        if (vkGetSwapchainImagesKHR(device.m_LogicalDevice, m_NativeSwapChain, &imageCount, m_SwapChainImages.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to get swap chain images! :(");
        }
        
        // Creating swapchain imageViews
        m_SwapChainImageViews.resize(m_SwapChainImages.size());
        for (size_t i = 0; i < imageCount; i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_SwapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_ImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            if (vkCreateImageView(device.m_LogicalDevice, &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views! :(");
            }
        }

    }

    void SwapChain::cleanUp()
    {
        VkDevice& device = VRen::get().getNativeDevice();
        vkDestroySwapchainKHR(device, m_NativeSwapChain, nullptr);
        for (VkImageView imageView : m_SwapChainImageViews)
        {
            vkDestroyImageView(device, imageView, nullptr);
        }
        for (auto framebuffer : m_SwapChainFramebuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
    }

    void SwapChain::recreate()
    {
        //TO-DO Vulkan 1
        /*
        int width = 0, height = 0;
        glfwGetFramebufferSize(VRen::get().getGLFWwindow(), &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(VRen::get().getGLFWwindow(), &width, &height);
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(VRen::get().getNativeDevice());
        cleanUp(); 
        create();
        createFrameBuffers();*/
    }

    void SwapChain::createFrameBuffers()
    {
        Device& device = VRen::get().getDevice();
        m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());
        for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
            VkImageView attachments[] = {
                m_SwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = VRen::get().getRenderPass();
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = m_Extent.width;
            framebufferInfo.height = m_Extent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device.m_LogicalDevice, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer! :(");
            }
        }
    }

}