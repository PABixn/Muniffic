#include "egpch.h"
#include "VulkanTexture2D.h"
#include "VulkanImageFactory.h"
#include <filesystem>
namespace eg {
    Ref<Texture2D> VulkanTexture2D::Create(const std::string& path)
    {
        return Ref<Texture2D>();
    }
    Ref<Texture2D> VulkanTexture2D::Create(const UUID& id)
    {
        return Ref<Texture2D>();
    }
    Ref<Texture2D> VulkanTexture2D::Create(const TextureSpecification& specification)
    {
        return Ref<Texture2D>();
    }
    bool VulkanTexture2D::Load(const std::string& path)
    {
        m_Image = VulkanImageFactory::CreateImage(path);
    }
}
