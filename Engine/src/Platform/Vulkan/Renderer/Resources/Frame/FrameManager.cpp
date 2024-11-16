#include "FrameManager.h"
#include <iostream>
#include "Renderer/Device/Device.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include <imgui.h>
#include "Imgui/backends/imgui_impl_vulkan.h"
#include "Engine/Core/Application.h"

void eg::FrameManager::init(ResourceManager* rm)
{
	m_ResourceManagerRef = rm;
	createCommandBuffers();
	CreateSyncObjects();
	createSceneFramebuffer(1,1, false);
	createRenderPass();
}

void eg::FrameManager::cleanUp()
{
	Device& device = VRen::get().getDevice();
	vkDeviceWaitIdle(device.getNativeDevice());
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(device.getNativeDevice(), m_EditorImageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(device.getNativeDevice(), m_EditorRenderFinishedSemaphores[i], nullptr);
		vkDestroyFence(device.getNativeDevice(), m_EditorInFlightFences[i], nullptr);
		vkDestroySemaphore(device.getNativeDevice(), m_SceneRenderFinishedSemaphores[i], nullptr);
	}
	cleanUpFramebuffer();
	vkDestroyRenderPass(device.getNativeDevice(), m_SceneRenderPass, nullptr);
	vkDestroyCommandPool(device.getNativeDevice(), m_CommandPool, nullptr);
}

void eg::FrameManager::cleanUpFramebuffer()
{
	vkDeviceWaitIdle(VRen::get().getNativeDevice());
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT + 1; i++) {
		if (m_SceneRenderImageRendererID[i]!= nullptr){
			vkFreeDescriptorSets(VRen::get().getNativeDevice(), VRen::get().getImGuiDescriptorPool(), 1, (VkDescriptorSet*)&m_SceneRenderImageRendererID[i]);
		}
		if(m_SceneRenderSampler[i] != nullptr)
			vkDestroySampler(VRen::get().getNativeDevice(), m_SceneRenderSampler[i], nullptr);
		vkDestroyFramebuffer(VRen::get().getNativeDevice(), m_SceneRenderFramebuffers[i], nullptr);
		vkDestroyImageView(VRen::get().getNativeDevice(), m_SceneRenderImageViews[i], nullptr);
		vkFreeMemory(VRen::get().getNativeDevice(), m_SceneRenderImageMemory[i], nullptr);
		vkDestroyImage(VRen::get().getNativeDevice(), m_SceneRenderImages[i], nullptr);
	}
}

void eg::FrameManager::createCommandBuffers()
{
	Device& device = VRen::get().getDevice();
	QueueFamilyIndices queueFamilyIndices = Queue::findQueueFamilies(device.m_PhysicalDevice, device.m_Surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	if (vkCreateCommandPool(device.m_LogicalDevice, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool! :(");
	}
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_CommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

	if (vkAllocateCommandBuffers(device.m_LogicalDevice, &allocInfo, m_EditorCommandBuffers) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers! :(");
	}

	VkCommandBufferAllocateInfo allocInfo2{};
	allocInfo2.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo2.commandPool = m_CommandPool;
	allocInfo2.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo2.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

	if (vkAllocateCommandBuffers(device.m_LogicalDevice, &allocInfo2, m_SceneCommandBuffers) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers! :(");
	}
}

void eg::FrameManager::createSceneFramebuffer(uint32_t widthArg, uint32_t heightArg, bool ImguiInit)
{
	m_ViewportWidth = widthArg;
	m_ViewportHeight = heightArg;
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT+1; i++)
	{
		VkFormat imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = m_ViewportWidth;
		imageInfo.extent.height = m_ViewportHeight;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = imageFormat;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0;
		if (vkCreateImage(VRen::get().getNativeDevice(), &imageInfo, nullptr, &m_SceneRenderImages[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image! :(");
		}
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(VRen::get().getNativeDevice(), m_SceneRenderImages[i], &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(VRen::get().getNativeDevice(), &allocInfo, nullptr, &m_SceneRenderImageMemory[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory! :(");
		}

		vkBindImageMemory(VRen::get().getNativeDevice(), m_SceneRenderImages[i], m_SceneRenderImageMemory[i], 0);

		VRen::get().getResourceManager().m_TextureManager.TransitionTextureLayout(m_SceneRenderImages[i], imageFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);







		// Creating imageViews
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = m_SceneRenderImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = imageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		if (vkCreateImageView(VRen::get().getNativeDevice(), &createInfo, nullptr, &m_SceneRenderImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views! :(");
		}

		VkImageView attachments[] = {
			   m_SceneRenderImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = VRen::get().getEditorRenderPass();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = m_ViewportWidth;
		framebufferInfo.height = m_ViewportHeight;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(VRen::get().getNativeDevice(), &framebufferInfo, nullptr, &m_SceneRenderFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer! :(");
		}
		

	}
	if (ImguiInit) initForImGui();
}

void eg::FrameManager::createRenderPass()
{
	// Render pass utilities
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = VRen::get().getSwapChain().m_ImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

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

	if (vkCreateRenderPass(VRen::get().getNativeDevice(), &renderPassInfo, nullptr, &m_SceneRenderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}

void eg::FrameManager::drawEditorFrame()
{
	EG_PROFILE_FUNCTION();
	uint32_t imageIndex;
	VkResult result;
	{
		EG_PROFILE_SCOPE("Fences wait");
		vkWaitForFences(VRen::get().getNativeDevice(), 1, &m_EditorInFlightFences[m_CurrentFrameInFlightIndex], VK_TRUE, UINT64_MAX);
		result = vkAcquireNextImageKHR(VRen::get().getNativeDevice(), VRen::get().getSwapChain().m_NativeSwapChain, UINT64_MAX, m_EditorImageAvailableSemaphores[m_CurrentFrameInFlightIndex], VK_NULL_HANDLE, &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
#if WIN32
			VRen::get().getSwapChain().recreate((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
#else
			EG_CORE_ASSERT(false);// FOR NOW THE APP IS ONLY FOR WIDNOWS USING GLFW
#endif // WIN32
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}
		vkResetFences(VRen::get().getNativeDevice(), 1, &m_EditorInFlightFences[m_CurrentFrameInFlightIndex]);
	}
	m_ResourceManagerRef->m_DescriptorManager.updateUniformBuffer(m_CurrentFrameInFlightIndex);
	drawSceneFrame(imageIndex);
	{
		// Rendering the editor
		EG_PROFILE_SCOPE("Command buffer record");
		vkResetCommandBuffer(m_EditorCommandBuffers[m_CurrentFrameInFlightIndex], 0);
		editorRecordCommandBuffer(m_EditorCommandBuffers[m_CurrentFrameInFlightIndex], imageIndex);
	}
	{
		EG_PROFILE_SCOPE("Queue submission");
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_EditorImageAvailableSemaphores[m_CurrentFrameInFlightIndex];
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_EditorCommandBuffers[m_CurrentFrameInFlightIndex];

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_EditorRenderFinishedSemaphores[m_CurrentFrameInFlightIndex];

		if (vkQueueSubmit((*VRen::get().getDevice().m_GraphicsQueue.GetNativeQueue().get()), 1, &submitInfo, m_EditorInFlightFences[m_CurrentFrameInFlightIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}
	}
	{
		EG_PROFILE_SCOPE("Queue presentation");
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		std::vector<VkSemaphore> semaphores(1);
		if (VRen::get().getSceneRenderData().m_Loaded) {
			presentInfo.waitSemaphoreCount = 2;
			semaphores.resize(2);
			semaphores = { m_EditorRenderFinishedSemaphores[m_CurrentFrameInFlightIndex], m_SceneRenderFinishedSemaphores[m_CurrentFrameInFlightIndex] };
		}
		else
		{
			presentInfo.waitSemaphoreCount = 1;
			semaphores = { m_EditorRenderFinishedSemaphores[m_CurrentFrameInFlightIndex] };

		}
		presentInfo.pWaitSemaphores = semaphores.data();
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &VRen::get().getSwapChain().m_NativeSwapChain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		result = vkQueuePresentKHR((*VRen::get().getDevice().m_PresentQueue.GetNativeQueue().get()), &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
#if WIN32
			VRen::get().getSwapChain().recreate((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
#else
			EG_CORE_ASSERT(false);// FOR NOW THE APP IS ONLY FOR WIDNOWS USING GLFW
#endif // WIN32
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}
	}
	m_CurrentFrameInFlightIndex = (m_CurrentFrameInFlightIndex + 1) % MAX_FRAMES_IN_FLIGHT;

}

void eg::FrameManager::drawSceneFrame(uint32_t imageIndex)
{
	if (!VRen::get().getSceneRenderData().m_Loaded) return;
	EG_PROFILE_FUNCTION();
	{
		EG_PROFILE_SCOPE("Scene render: Layout Transition");

			VRen::get().getResourceManager().m_TextureManager.TransitionTextureLayout(m_SceneRenderImages[imageIndex], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	}
	{
		EG_PROFILE_SCOPE("Scene render: CommandBuffer record")
			vkResetCommandBuffer(m_SceneCommandBuffers[m_CurrentFrameInFlightIndex], 0);
		sceneRecordCommandBuffer(m_SceneCommandBuffers[m_CurrentFrameInFlightIndex], imageIndex);
	}

	{
		EG_PROFILE_SCOPE("Scene redner: Queue submission");
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_SceneCommandBuffers[m_CurrentFrameInFlightIndex];

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_SceneRenderFinishedSemaphores[m_CurrentFrameInFlightIndex];

		if (vkQueueSubmit((*VRen::get().getDevice().m_GraphicsQueue.GetNativeQueue().get()), 1, &submitInfo, nullptr) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}
	}
	{
		EG_PROFILE_SCOPE("Scene render: Layout Transition");

		VRen::get().getResourceManager().m_TextureManager.TransitionTextureLayout(m_SceneRenderImages[imageIndex], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

}

void eg::FrameManager::initForImGui()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT + 1; i++) {
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

			if (vkCreateSampler(VRen::get().getNativeDevice(), &samplerInfo, nullptr, &m_SceneRenderSampler[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create texture sampler!");
			}
		}
		{
			EG_PROFILE_SCOPE("VulkanTexture: ImGui descriptorSet creation");
			m_SceneRenderImageRendererID[i] = ImGui_ImplVulkan_AddTexture(m_SceneRenderSampler[i], m_SceneRenderImageViews[i], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}
	}
}

void eg::FrameManager::resizeViewport(const std::pair<uint32_t, uint32_t>& viewportSize)
{
	cleanUpFramebuffer();
	createSceneFramebuffer(viewportSize.first, viewportSize.second);
}

void eg::FrameManager::editorRecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}
	VkRenderPassBeginInfo renderPassInfo{};

	// ImGui Render
	renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = VRen::get().getEditorRenderPass();
	renderPassInfo.framebuffer = VRen::get().getSwapChain().m_SwapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = VRen::get().getSwapChain().getSwapChainExtent();
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

	vkCmdEndRenderPass(commandBuffer);



	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer! :(");
	}
}

void eg::FrameManager::sceneRecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}
	VkRenderPassBeginInfo renderPassInfo{};

	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_SceneRenderPass;
	renderPassInfo.framebuffer = m_SceneRenderFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = { m_ViewportWidth, m_ViewportHeight };
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VRen::get().getGraphicsPipeline().getNativePipeline());


	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(m_ViewportWidth);
	viewport.height = static_cast<float>(m_ViewportHeight);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = { m_ViewportWidth, m_ViewportHeight };
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	VkDescriptorSet sets[] = { m_ResourceManagerRef->getDescriptorSets()[m_CurrentFrameInFlightIndex], VRen::get().getSceneRenderData().getMatrixBufferDescriptorHelper().m_DescriptorSet };
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VRen::get().getGraphicsPipeline().getPipelineLayout(), 0, 2, sets, 0, nullptr);



	std::vector<VkBuffer> vertexBufferss = { VRen::get().getSceneRenderData().m_VertexBuffer.m_Buffer.m_Buffer };
	VkDeviceSize offsetss[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBufferss.data(), offsetss);
	vkCmdBindIndexBuffer(commandBuffer, VRen::get().getSceneRenderData().m_IndexBuffer.m_Buffer.m_Buffer, 0, VK_INDEX_TYPE_UINT32);
	vkCmdDrawIndexed(commandBuffer, VRen::get().getSceneRenderData().m_IndexBuffer.m_IndicesCount, VRen::get().getSceneRenderData().m_SSBO.m_InstancesCount, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer! :(");
	}
}


void eg::FrameManager::CreateSyncObjects()
{
	EG_PROFILE_FUNCTION();
	{
		EG_PROFILE_SCOPE("editor sync obj")
		VkDevice& device = VRen::get().getNativeDevice();
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_EditorImageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_EditorRenderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(device, &fenceInfo, nullptr, &m_EditorInFlightFences[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create semaphores and/or fences! :(");
			}
		}
	}
	{
		EG_PROFILE_SCOPE("scene sync obj");
		VkDevice& device = VRen::get().getNativeDevice();
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		for (size_t i = 0; i < std::size(m_SceneRenderFinishedSemaphores) ; i++) {
			if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_SceneRenderFinishedSemaphores[i]) != VK_SUCCESS){
				throw std::runtime_error("failed to create semaphores and/or fences! :(");
			}
		}
	}
}
