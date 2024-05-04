#include "egpch.h"
#include "VulkanHandler.h"
#include "VulkanCommandManager.h"
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

	VulkanHandler::VulkanHandler(GLFWwindow* window):m_Window(window)
	{
	}

	void VulkanHandler::DrawFrame()
	{
	}

	void VulkanHandler::Cleanup()
	{
		m_SwapChain.CleanupSwapChain();
		m_GraphicsPipeline.Cleanup();
		m_MainRenderPass.Cleanup(m_LogicalDevice.GetDevice());
		m_CommandPool.Cleanup();
		
		m_LogicalDevice.Cleanup();

		m_Surface.Cleanup(m_Instance.GetInstance());
		m_Instance.Cleanup();
	}

	VulkanHandler::~VulkanHandler()
	{
		Cleanup();
	}

}
