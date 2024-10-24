#include "VulkanTexture.h"
#include <stb_image.h>
#include "Platform/Vulkan/VulkanRenderer.h"
#include "Engine/Core/Core.h"
#include "Imgui/backends/imgui_impl_vulkan.h"

eg::Ref<eg::Texture2D> eg::VulkanTexture::Create(const TextureSpecification& specification)
{
    EG_ASSERT(false);
    return nullptr;
}

eg::Ref<eg::Texture2D> eg::VulkanTexture::Create(const std::string& path)
{
    return CreateRef<VulkanTexture>(path.c_str());
}

eg::Ref<eg::Texture2D> eg::VulkanTexture::Create(const UUID& id)
{
    EG_ASSERT(false);
    return nullptr;
}

eg::VulkanTexture::VulkanTexture(const char* path)
{
    LoadTexture(&VRen::get().getResourceManager().getTextureManager(), path);
}

void eg::VulkanTexture::cleanUp()
{
    vkDestroySampler(VRen::get().getNativeDevice(), m_Sampler, nullptr);
    vkDestroyImageView(VRen::get().getNativeDevice(), m_TextureImageView, nullptr);
    vkDestroyImage(VRen::get().getNativeDevice(), m_TextureImage, nullptr);
    vkFreeMemory(VRen::get().getNativeDevice(), m_TextureImageMemory, nullptr);
}

void eg::VulkanTexture::LoadTexture(TextureManager* textureManager, const char* Argpath){
    EG_PROFILE_FUNCTION();
    {
        EG_PROFILE_SCOPE("VulkanTexture: texels load");
        this->path = Argpath;
        Device& device = VRen::get().getDevice();
        VulkanBuffer stagingBuffer;
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(Argpath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;
        if (!pixels) {
            throw std::runtime_error("failed to load texture image! :(");
        }
        EG_ASSERT(texWidth >= 0 && texHeight >= 0);
        this->width = (uint32_t)texWidth;
        this->height = (uint32_t)texHeight;

        stagingBuffer = VRen::get().getResourceManager().createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        void* data;
        vkMapMemory(device.getNativeDevice(), stagingBuffer.m_Memory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(device.getNativeDevice(), stagingBuffer.m_Memory);
        stbi_image_free(pixels);

        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = static_cast<uint32_t>(texWidth);
        imageInfo.extent.height = static_cast<uint32_t>(texHeight);
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = 0;
        if (vkCreateImage(device.getNativeDevice(), &imageInfo, nullptr, &m_TextureImage) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image! :(");
        }
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device.getNativeDevice(), m_TextureImage, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (vkAllocateMemory(device.getNativeDevice(), &allocInfo, nullptr, &m_TextureImageMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory! :(");
        }
        vkBindImageMemory(device.getNativeDevice(), m_TextureImage, m_TextureImageMemory, 0);

        textureManager->TransitionTextureLayout(*this, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        textureManager->CopyBufferToImage(stagingBuffer, *this);
        textureManager->TransitionTextureLayout(*this, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        VRen::get().getResourceManager().DestroyBuffer(stagingBuffer);
    }
    {
        EG_PROFILE_SCOPE("VulkanTexture: image view creation");
        Device& device = VRen::get().getDevice();
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_TextureImage;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device.getNativeDevice(), &viewInfo, nullptr, &m_TextureImageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }
    }
    {
        EG_PROFILE_SCOPE("VulkanTexture: sampler creation");
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(VRen::get().getDevice().m_PhysicalDevice, &properties);

        samplerInfo.anisotropyEnable = VK_TRUE; 
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(VRen::get().getNativeDevice(), &samplerInfo, nullptr, &m_Sampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }
    this->rendererID = ImGui_ImplVulkan_AddTexture(m_Sampler, m_TextureImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    this->loaded = true;
}

ImTextureID eg::VulkanTexture::GetRendererID() const
{
    return rendererID;
}

