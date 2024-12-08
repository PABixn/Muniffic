#include "GraphicsPipeline.h"
#include "ObjectWithoutTexturePipeline.h"

void eg::PipelineManager::CreatePipeline(const GraphicsPipelineType& type)
{
	if (m_Pipelines.find(type)==m_Pipelines.end())
	{
		if (type == GraphicsPipelineType::Type_BasicMeshWithColor)
		{
			m_Pipelines[GraphicsPipelineType::Type_BasicMeshWithColor] = ObjectWithoutTexturePipeline();
			m_Pipelines[GraphicsPipelineType::Type_BasicMeshWithColor].init();
		}
	}
}

eg::GraphicsPipeline* eg::PipelineManager::getPipeline(const GraphicsPipelineType& type)
{
	if (m_Pipelines.find(type) != m_Pipelines.end()) {
		return &m_Pipelines[type];
	}
	else
	{
		EG_ASSERT(false, "attempt to get unexisting pipeline");
	}

}
