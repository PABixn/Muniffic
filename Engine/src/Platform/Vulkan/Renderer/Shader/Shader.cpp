#include "Shader.h"
#include <fstream>
#include "Renderer/Device/Device.h"
#include "Platform/Vulkan/VulkanRenderer.h"
std::vector<char> readFile(const std::filesystem::path& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file! :(");
    }
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}
void eg::VulkanShader::LoadShader(std::filesystem::path path)
{
    auto code = readFile(path);
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    if (vkCreateShaderModule(VRen::get().getNativeDevice(), &createInfo, nullptr, &m_NativeShaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module! :(");
    }
}

void eg::VulkanShader::CleanUp()
{
    vkDestroyShaderModule(VRen::get().getNativeDevice(), m_NativeShaderModule, nullptr);
}
