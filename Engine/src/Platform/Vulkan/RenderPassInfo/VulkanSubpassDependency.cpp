#include "egpch.h"
#include "VulkanSubpassDependency.h"

namespace eg {

    VulkanSubpassDependency& VulkanSubpassDependency::CreateSubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass)
    {
        VulkanSubpassDependency dependency;
        dependency.m_SubpassDependency.srcSubpass = srcSubpass;
        dependency.m_SubpassDependency.dstSubpass = dstSubpass;
        dependency.m_SubpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.m_SubpassDependency.srcAccessMask = 0;
        dependency.m_SubpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.m_SubpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        return dependency;
    }

    VulkanSubpassDependency& VulkanSubpassDependency::CreateSubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDependencyFlags dependencyFlags)
    {
        VulkanSubpassDependency dependency;
        dependency.m_SubpassDependency.srcSubpass = srcSubpass;
        dependency.m_SubpassDependency.dstSubpass = dstSubpass;
        dependency.m_SubpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.m_SubpassDependency.srcAccessMask = srcAccessMask;
        dependency.m_SubpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.m_SubpassDependency.dstAccessMask = dstAccessMask;
        dependency.m_SubpassDependency.dependencyFlags = dependencyFlags;

        return dependency;
    }

    VulkanSubpassDependency& VulkanSubpassDependency::CreateSubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDependencyFlags dependencyFlags, uint32_t srcStageMask, uint32_t dstStageMask)
    {
        VulkanSubpassDependency dependency;
        dependency.m_SubpassDependency.srcSubpass = srcSubpass;
        dependency.m_SubpassDependency.dstSubpass = dstSubpass;
        dependency.m_SubpassDependency.srcStageMask = srcStageMask;
        dependency.m_SubpassDependency.srcAccessMask = srcAccessMask;
        dependency.m_SubpassDependency.dstStageMask = dstStageMask;
        dependency.m_SubpassDependency.dstAccessMask = dstAccessMask;
        dependency.m_SubpassDependency.dependencyFlags = dependencyFlags;

        return dependency;
    }

}
