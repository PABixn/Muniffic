#include "TextureDescriptor.h"
#include "VulkanRenderer.h"
#include "Platform/Vulkan/Renderer/Resources/Buffer/SamplerArray.h"
void eg::TextureDescriptorHelper::init()
{
	VkDescriptorPoolSize poolSize = {};
	poolSize.type = VK_DESCRIPTOR_TYPE_SAMPLER;
	poolSize.descriptorCount = 8;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = 1;

	vkCreateDescriptorPool(VRen::get().getNativeDevice(), &poolInfo, nullptr, &m_DescriptorPool);


	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.binding = 2;
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	layoutBinding.descriptorCount = 8;
	layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &layoutBinding;

	VkResult res = vkCreateDescriptorSetLayout(VRen::get().getNativeDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout);
	EG_ASSERT(res == VK_SUCCESS);

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_DescriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &m_DescriptorSetLayout;

	vkAllocateDescriptorSets(VRen::get().getNativeDevice(), &allocInfo, &m_DescriptorSet);
}

void eg::TextureDescriptorHelper::cleanup()
{
	vkDestroyDescriptorSetLayout(VRen::get().getNativeDevice(), m_DescriptorSetLayout, nullptr);
	vkDestroyDescriptorPool(VRen::get().getNativeDevice(), m_DescriptorPool, nullptr);
}
void eg::TextureDescriptorHelper::bindSamplerArray(VulkanSamplerArray* samplerarray)
{
	EG_ASSERT(samplerarray != nullptr);
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = samplerarray->m_Buffer.m_Buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = VK_WHOLE_SIZE;

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = m_DescriptorSet;
	descriptorWrite.dstBinding = 1;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(VRen::get().getNativeDevice(), 1, &descriptorWrite, 0, nullptr);
}
/*
void eg::TextureDescriptorHelper::bindSSBO(VulkanShaderStorageBuffer* SSBO)
{
	EG_ASSERT(SSBO != nullptr);
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = SSBO->m_Buffer.m_Buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = VK_WHOLE_SIZE;

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = m_DescriptorSet;
	descriptorWrite.dstBinding = 1;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(VRen::get().getNativeDevice(), 1, &descriptorWrite, 0, nullptr);
}
*/