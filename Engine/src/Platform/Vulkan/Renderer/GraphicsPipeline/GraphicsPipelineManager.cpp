#include "GraphicsPipelineManager.h"

void eg::PipelineManager::CreatePipeline(const GraphicsPipelineType& type)
{
	if (type == GraphicsPipelineType::Type_BasicMeshWithColor)
	{
		if (m_PureColorPipeline.getNativePipeline()==VK_NULL_HANDLE)
		{
			m_PureColorPipeline.init();
		}
		else
		{
			EG_ASSERT(false, "Create pipeline shouldn't be called twice");
		}
	}
	else
	{
		EG_ASSERT(false);
	}
}

eg::GraphicsPipeline* eg::PipelineManager::getPipeline(const GraphicsPipelineType& type)
{
	if (type == GraphicsPipelineType::Type_BasicMeshWithColor)
	{
		return &m_PureColorPipeline;
	}
	EG_ASSERT(false);
}

void eg::PipelineManager::cleanUpPipeline(const GraphicsPipelineType& type)
{
	if (type == GraphicsPipelineType::Type_BasicMeshWithColor)
	{
		m_PureColorPipeline.cleanUp();
	}
	EG_ASSERT(false);
}

void eg::PipelineManager::cleanUpAllPipelines()
{
	if (m_PureColorPipeline.getNativePipeline() != nullptr)
	{
		m_PureColorPipeline.cleanUp();
	}
}
