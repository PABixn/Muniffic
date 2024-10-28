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
	createCommandBuffer();
	CreateSyncObjects();
}

void eg::FrameManager::cleanUp()
{
	Device& device = VRen::get().getDevice();
	vkDeviceWaitIdle(device.getNativeDevice());
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(device.getNativeDevice(), m_ImageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(device.getNativeDevice(), m_RenderFinishedSemaphores[i], nullptr);
		vkDestroyFence(device.getNativeDevice(), m_InFlightFences[i], nullptr);
	}
	vkDestroyCommandPool(device.getNativeDevice(), m_CommandPool, nullptr);
}

void eg::FrameManager::createCommandBuffer()
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
	allocInfo.commandBufferCount = 2;

	if (vkAllocateCommandBuffers(device.m_LogicalDevice, &allocInfo, m_CommandBuffers) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers! :(");
	}
}

void eg::FrameManager::drawFrame()
{
	uint32_t imageIndex;
	VkResult result;
	{
		EG_PROFILE_SCOPE("Fences wait");
		vkWaitForFences(VRen::get().getNativeDevice(), 1, &m_InFlightFences[m_CurrentFrameInFlightIndex], VK_TRUE, UINT64_MAX);
		result = vkAcquireNextImageKHR(VRen::get().getNativeDevice(), VRen::get().getSwapChain().m_NativeSwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrameInFlightIndex], VK_NULL_HANDLE, &imageIndex);
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
		vkResetFences(VRen::get().getNativeDevice(), 1, &m_InFlightFences[m_CurrentFrameInFlightIndex]);
	}
	m_ResourceManagerRef->m_DescriptorManager.updateUniformBuffer(m_CurrentFrameInFlightIndex);
	{
		EG_PROFILE_SCOPE("Command buffer record");
		vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrameInFlightIndex], 0);
		recordCommandBuffer(m_CommandBuffers[m_CurrentFrameInFlightIndex], imageIndex);
	}
	{
		EG_PROFILE_SCOPE("Queue submission");
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphores[m_CurrentFrameInFlightIndex];
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrameInFlightIndex];

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_RenderFinishedSemaphores[m_CurrentFrameInFlightIndex];

		if (vkQueueSubmit((*VRen::get().getDevice().m_GraphicsQueue.GetNativeQueue().get()), 1, &submitInfo, m_InFlightFences[m_CurrentFrameInFlightIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}
	}
	{
		EG_PROFILE_SCOPE("Queue presentation");
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_RenderFinishedSemaphores[m_CurrentFrameInFlightIndex];
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

void eg::FrameManager::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}
	// Scene Render
	VkRenderPassBeginInfo renderPassInfo{};
	/*
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = VRen::get().getRenderPass();
	renderPassInfo.framebuffer = VRen::get().getSwapChain().m_SwapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = VRen::get().getSwapChain().getSwapChainExtent();
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VRen::get().getGraphicsPipeline().getNativePipeline());


	std::vector<VkBuffer> vertexBuffers =  m_ResourceManagerRef->getVertexBuffers();
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers.data(), offsets);
	vkCmdBindIndexBuffer(commandBuffer, m_ResourceManagerRef->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);


	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(VRen::get().getSwapChain().getSwapChainExtent().width);
	viewport.height = static_cast<float>(VRen::get().getSwapChain().getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = VRen::get().getSwapChain().getSwapChainExtent();
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VRen::get().getGraphicsPipeline().getPipelineLayout(), 0, 1, &m_ResourceManagerRef->getDescriptorSets()[m_CurrentFrameInFlightIndex], 0, nullptr);


	vkCmdDrawIndexed(commandBuffer, m_ResourceManagerRef->getIndicesCount(), 1, 0, 0, 0);
	vkCmdEndRenderPass(commandBuffer);
	*/
	// ImGui Render
	renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = VRen::get().getImGuiRenderPass();
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


void eg::FrameManager::CreateSyncObjects()
{
	VkDevice& device = VRen::get().getNativeDevice();
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores and/or fences! :(");
		}
	}

}
