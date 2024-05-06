#include "egpch.h"
#include "VulkanHandler.h"
#include "VulkanCommandManager.h"
#include "Engine/Renderer/RenderCommand.h"
#include "VulkanRenderAPI.h"
namespace eg {
	
	Ref<VulkanHandler> VulkanHandler::Create(GLFWwindow* window)
	{
		Ref<VulkanHandler> handler = CreateRef<VulkanHandler>(window);
		handler->Init();
		return handler;
	}

	void VulkanHandler::Init()
	{
		//this is a test solution later on Might need to be changed to a better solution(creating everything through scoped pointers or reference counting pointers as custom allocator)
		EG_ASSERT(VulkanValidationLayers::GetInstance().ValidationLayersEnabled() && !VulkanValidationLayers::GetInstance().CheckValidationLayerSupport(), "Validation layers requested, but not available!");
		m_Instance.Init(m_DeviceExtensions);
		m_Surface.Init(m_Instance.GetInstance(), m_Window);
		m_PhysicalDevice.Init(m_Instance.GetInstance(), m_Surface.GetSurface(), m_DeviceExtensions);
		m_LogicalDevice.Init(m_PhysicalDevice.GetPhysicalDevice(), m_Surface.GetSurface(), m_DeviceExtensions);
		VulkanCommandManager::Init(m_LogicalDevice.GetDevice(), m_LogicalDevice.GetGraphicsQueue(), m_CommandPool.GetPool());
		VulkanDeviceMemoryManager::Init(m_LogicalDevice.GetDevice(), m_PhysicalDevice.GetPhysicalDevice());
		m_SwapChain.Init();
		m_MainRenderPass.Init();
		m_CommandPool.Create();
		m_SwapChain.CreateColorResources();
		m_SwapChain.CreateDepthResources();
		m_SwapChain.CreateFramebuffers();

	}

	VulkanHandler::VulkanHandler(GLFWwindow* window) :m_Window(window)
	{
	}

	void VulkanHandler::CreateCommandBuffers()
	{
		m_commandBuffers.resize(m_SwapChain.GetFramebuffers().size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool.GetPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

		if (vkAllocateCommandBuffers(m_LogicalDevice.GetDevice(), &allocInfo, (VkCommandBuffer*)m_commandBuffers.data()) != VK_SUCCESS) {
			EG_ASSERT("failed to allocate command buffers!");
		}
	}

	void VulkanHandler::CreateSyncObjects()
	{
		m_ImageAvailableSemaphores.resize(VulkanConstants::MAX_FRAMES_IN_FLIGHT);
		m_RenderFinishedSemaphores.resize(VulkanConstants::MAX_FRAMES_IN_FLIGHT);

		m_InFlightFences.resize(VulkanConstants::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < VulkanConstants::MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_ImageAvailableSemaphores[i] = VulkanSemaphore(m_LogicalDevice.GetDevice());
			m_RenderFinishedSemaphores[i] = VulkanSemaphore(m_LogicalDevice.GetDevice());
			m_InFlightFences[i] = VulkanFence(m_LogicalDevice.GetDevice());
		}
	}

	void VulkanHandler::StartFrame()
	{
		vkWaitForFences(m_LogicalDevice, 1, m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

		VkResult result = vkAcquireNextImageKHR(m_LogicalDevice, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &m_NextImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			//Might need to recreate swapchain in VulkanHandler class not in SwapChain class
			m_SwapChain.RecreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		vkResetFences(m_LogicalDevice, 1, m_InFlightFences[m_CurrentFrame]);
		vkResetCommandBuffer(m_commandBuffers[m_CurrentFrame], 0);
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(m_commandBuffers[m_CurrentFrame], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		//Little hacky way to set the command buffer for the renderer API
		((VulkanRendererAPI*)RenderCommand::GetRendererAPI())->SetCommandBuffer(&m_commandBuffers[m_CurrentFrame]);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_MainRenderPass;
		renderPassInfo.framebuffer = m_SwapChain.GetFramebuffers()[m_NextImageIndex];//swapChainFrameBuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = m_SwapChain.GetExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_commandBuffers[m_CurrentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_SwapChain.GetExtent().width);
		viewport.height = static_cast<float>(m_SwapChain.GetExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_commandBuffers[m_CurrentFrame], 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapChain.GetExtent();
		vkCmdSetScissor(m_commandBuffers[m_CurrentFrame], 0, 1, &scissor);
	}

	

	void VulkanHandler::DrawFrame()
	{

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;


		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = m_commandBuffers[m_CurrentFrame];

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(m_LogicalDevice.GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_SwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &m_NextImageIndex;

		VkResult result = vkQueuePresentKHR(m_LogicalDevice.GetPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			//frameBufferResized = false;
			m_SwapChain.RecreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		m_CurrentFrame = (m_CurrentFrame + 1) % VulkanConstants::MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanHandler::Cleanup()
	{
		m_SwapChain.CleanupSwapChain();
		m_MainRenderPass.Cleanup(m_LogicalDevice.GetDevice());
		m_CommandPool.Cleanup();
		
		m_LogicalDevice.Cleanup();

		m_Surface.Cleanup(m_Instance.GetInstance());
		m_Instance.Cleanup();
	}

	

	void VulkanHandler::RecordCommandBuffer(VkCommandBuffer& commandBuffer, uint32_t imageIndex)
	{
		


		
		//Abstract this part
		//Drawing stuff goes here
		//
		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}

	VulkanHandler::~VulkanHandler()
	{
		Cleanup();
	}

}
