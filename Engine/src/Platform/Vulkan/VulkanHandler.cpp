#include "egpch.h"
#include "VulkanHandler.h"

namespace eg {

	

	void VulkanHandler::Init()
	{
		EG_ASSERT(m_ValidationLayers.ValidationLayersEnabled() && !m_ValidationLayers.CheckValidationLayerSupport(), "Validation layers requested, but not available!");
		m_Instance.Init();
		m_DebugMessenger.SetupDebugMessenger(m_Instance.GetInstance());
		//this is a test solution later on Might need to be changed to a better solution(creating everything through scoped pointers)
		CreateSurface();
		m_PhysicalDevice.PickPhysicalDevice(m_Instance.GetInstance(), m_Surface.GetSurface());
	}

	void VulkanHandler::Cleanup()
	{
		
		m_Instance.Cleanup();
	}



	void VulkanHandler::CreateSurface()
	{
		m_Surface = VulkanSurface(m_Instance.GetInstance());
	}

	VulkanHandler::~VulkanHandler()
	{
	}

}
