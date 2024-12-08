#include "egpch.h"
#include "VulkanRenderer.h"
#include "Engine/Core/Application.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Imgui/backends/imgui_impl_vulkan.h"
#include "Imgui/backends/imgui_impl_glfw.h"
#include "Renderer/info.h"
#include "Platform/Vulkan/Renderer/Resources/Scene/SceneRenderData.h"
#include "Engine/Scene/Scene.h"
eg::VulkanRenderer eg::VRen::vren = VulkanRenderer();

void eg::VulkanRenderer::init()
{
#if WIN32
    GLFWwindow* win = (GLFWwindow*) Application::Get().GetWindow().GetNativeWindow();
#else
    EG_CORE_ASSERT(false);// FOR NOW THE APP IS ONLY FOR WIDNOWS USING GLFW
#endif // WIN32
    m_Device.init(win);
    createCommandPoolForOneTimeOperations();
    m_SwapChain.create(win);
    createRenderPass();
    m_SwapChain.createFrameBuffers();
    m_ResourceManager.init();
    m_CurrentSceneRenderData.createBuffers(4, 10);
    m_GraphicsPipeline.init();
    m_ResourceManager.m_FrameManager.init(&m_ResourceManager);
    EG_TRACE("Vulkan Renderer initialization Succesful");
}

void eg::VulkanRenderer::cleanUp()
{
    vkDeviceWaitIdle(m_Device.getNativeDevice());
    m_CurrentSceneRenderData.unloadScene();
    m_ResourceManager.m_FrameManager.cleanUp();
    m_ResourceManager.cleanUp();
    m_GraphicsPipeline.cleanUp();
    m_SwapChain.cleanUp();
    //vkDestroyRenderPass(m_Device.getNativeDevice(), m_ImGuiRenderPass, nullptr);
    vkDestroyDescriptorPool(m_Device.getNativeDevice(), m_ImGuiDescriptorPool, nullptr);
    vkDestroyCommandPool(m_Device.getNativeDevice(), m_PoolForOneTimeOperations, nullptr);
    m_Device.cleanUp();
    EG_TRACE("Vulkan Renderer Deinitialization Succesful");
}

void eg::VulkanRenderer::render()
{
    m_ResourceManager.m_FrameManager.drawEditorFrame();
}

void eg::VulkanRenderer::LoadSceneData(Scene* scene)
{
    m_CurrentSceneRenderData.loadScene(scene);
}

void eg::VulkanRenderer::createCommandPoolForOneTimeOperations()
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = VRen::get().getDevice().m_GraphicsQueue.m_QueueFamilyIndices.graphicsFamily.value();
    if (vkCreateCommandPool(VRen::get().getNativeDevice(), &poolInfo, nullptr, &m_PoolForOneTimeOperations) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool! :(");
    }
}

void eg::VulkanRenderer::createRenderPass()
{
    // Render pass utilities
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = m_SwapChain.m_ImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; 
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    // Subpass dependencies for layout transitions
    VkSubpassDependency dependencyStart{};
    dependencyStart.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencyStart.dstSubpass = 0;
    dependencyStart.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencyStart.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencyStart.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencyStart.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkSubpassDependency dependencyEnd{};
    dependencyEnd.srcSubpass = 0;
    dependencyEnd.dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencyEnd.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencyEnd.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencyEnd.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencyEnd.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

    VkSubpassDependency dependencies[] = { dependencyStart, dependencyEnd };
    // Render pass creation
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = std::size(dependencies);
    renderPassInfo.pDependencies = dependencies;

    if (vkCreateRenderPass(m_Device.m_LogicalDevice, &renderPassInfo, nullptr, &m_EditorRenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

// ImGui Helper
static void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[ImGui vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}
void eg::VulkanRenderer::initImGui(GLFWwindow* window)
{
    EG_PROFILE_FUNCTION()
    {
        EG_PROFILE_SCOPE("ImGuiDescriptorPool creation")
            VkDescriptorPoolSize pool_sizes[] = {
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 100;
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        VkResult err = vkCreateDescriptorPool(m_Device.getNativeDevice(), &pool_info, nullptr, &m_ImGuiDescriptorPool);
        check_vk_result(err);
    }
    {
        EG_PROFILE_SCOPE("ImGui internal init")
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = m_Device.m_VulkanInstance;
        init_info.PhysicalDevice = m_Device.m_PhysicalDevice;
        init_info.Device = m_Device.m_LogicalDevice;
        init_info.QueueFamily = m_Device.m_GraphicsQueue.m_QueueFamilyIndices.graphicsFamily.value();
        init_info.Queue = *m_Device.m_GraphicsQueue.m_VulkanQueue.get();
        init_info.DescriptorPool = m_ImGuiDescriptorPool;
        init_info.RenderPass = m_EditorRenderPass;
        init_info.MinImageCount = 2;
        init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info);
    }
    m_ResourceManager.m_FrameManager.initForImGui();
}

VkCommandBuffer eg::VulkanRenderer::BeginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_PoolForOneTimeOperations;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(VRen::get().getNativeDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    return commandBuffer;
}

void eg::VulkanRenderer::EndSingleTimeCommands(VkCommandBuffer& cb)
{
    vkEndCommandBuffer(cb);
    m_Device.m_GraphicsQueue.submitToQueue(cb);
    vkFreeCommandBuffers(m_Device.getNativeDevice(), m_PoolForOneTimeOperations, 1, &cb);
}