#include "egpch.h"
#include "VulkanHandler.h"

namespace eg {

	VulkanHandler::VulkanHandler()
	{
	}

	void VulkanHandler::Init()
	{
		EG_ASSERT(m_ValidationLayers.ValidationLayersEnabled() && !m_ValidationLayers.CheckValidationLayerSupport(), "Validation layers requested, but not available!");
		m_Instance.Init();
		m_DebugMessenger.SetupDebugMessenger(m_Instance.GetInstance());
		m_Surface.CreateSurface(m_Instance.GetInstance());
		m_PhysicalDevice.PickPhysicalDevice(m_Instance.GetInstance(), m_Surface.GetSurface());
	}

	void VulkanHandler::Cleanup()
	{
		
		m_Instance.Cleanup();
	}



	VulkanHandler::~VulkanHandler()
	{
	}

}
