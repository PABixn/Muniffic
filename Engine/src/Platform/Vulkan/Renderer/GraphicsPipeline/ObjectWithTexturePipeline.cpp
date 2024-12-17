#include "ObjectWithoutTexturePipeline.h"
#include "Renderer/Device/Device.h"
#include "Renderer/Queue/Queue.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "Renderer/Resources/Vertex/Vertex.h"
#define VERTEX_SHADER_PATH SHADERS_FOLDER "/textureBasicObject/vert.spv"
#define FRAGMENT_SHADER_PATH SHADERS_FOLDER "/textureBasicObject/frag.spv"

void eg::ObjectWithoutTexturePipeline::init()
{
	// Graphics pipeline:
	//	Vertex data
	//		|
	//		v
	//	Input assembler
	//		|
	//		v
	//	Vertex shader
	//		|
	//		v
	//	Tessellation
	//		|
	//		v
	//	Geometry shader
	//		|
	//		v
	//	Rasterization
	//		|
	//		v
	//	Fragment shader
	//		|
	//		v
	//	Color blending
	//		|
	//		v
	//	Framebuffer

	Device& device = VRen::get().getDevice();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	VkVertexInputBindingDescription vertexBindDesc = VulkanBasicMeshVertex::getBindingDescription();
	vertexInputInfo.pVertexBindingDescriptions = &vertexBindDesc;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(VulkanBasicMeshVertex::getAttributeDescriptions().size());
	vertexInputInfo.pVertexAttributeDescriptions = VulkanBasicMeshVertex::getAttributeDescriptions().data();


	m_VertexShader.LoadShader(std::filesystem::path(VERTEX_SHADER_PATH));
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = m_VertexShader.getShaderModule();
	vertShaderStageInfo.pName = "main";

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	defineInputAssembly(inputAssembly);

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	defineRasterizer(rasterizer);

	VkPipelineMultisampleStateCreateInfo multisampling{};
	defineMultisampling(multisampling);

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	VkPipelineColorBlendStateCreateInfo colorBlending{};
	defineColorBlend(colorBlendAttachment, colorBlending);

	m_FragmentShader.LoadShader(std::filesystem::path(FRAGMENT_SHADER_PATH));
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = m_FragmentShader.getShaderModule();
	fragShaderStageInfo.pName = "main";


	VkPipelineDynamicStateCreateInfo dynamicState{};
	defineDynamicStates(dynamicState);

	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineViewportStateCreateInfo viewportState{};
	defineViewportState(viewportState, viewport, scissor);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	VkDescriptorSetLayout layouts[] = { VRen::get().getResourceManager().getDescriptorSetLayout(), VRen::get().getSceneRenderData().getMatrixBufferDescriptorHelper().getDescriptorSetLayout(), VRen::get().getSceneRenderData().getTextureDescriptorHelper().getDescriptorSetLayout() };
	pipelineLayoutInfo.setLayoutCount = sizeof(layouts) / sizeof(layouts[0]);
	pipelineLayoutInfo.pSetLayouts = layouts;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;	



	if (vkCreatePipelineLayout(device.m_LogicalDevice, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout! :(");
	}


	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = m_PipelineLayout;
	pipelineInfo.renderPass = VRen::get().getEditorRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(device.m_LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_NativePipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline! :(");
	}
}

void eg::ObjectWithoutTexturePipeline::cleanUp()
{

	VkDevice& device = VRen::get().getNativeDevice();
	vkDestroyPipeline(device, m_NativePipeline, nullptr);
	vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
	vkDestroyRenderPass(device, VRen::get().getEditorRenderPass(), nullptr);
	m_FragmentShader.CleanUp();
	m_VertexShader.CleanUp();
}

void eg::ObjectWithoutTexturePipeline::onViewportResize(VkCommandBuffer& cmdBfr, const VkExtent2D& extentArg)
{
	static VkExtent2D CurrentExtent = { 0,0 };
	if (extentArg.width == CurrentExtent.width && extentArg.height == CurrentExtent.height) return;
	CurrentExtent = extentArg;
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(extentArg.width);
	viewport.height = static_cast<float>(extentArg.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	vkCmdSetViewport(cmdBfr, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = extentArg;

	vkCmdSetScissor(cmdBfr, 0, 1, &scissor);
}
