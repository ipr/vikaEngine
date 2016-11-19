// vikaPipeline : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaPipeline.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaPipeline::vikaPipeline(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_pipelineLayout(VK_NULL_HANDLE)
{
	// TODO: when using textures, define sampler here
	m_layoutBinding.binding = 0;
	m_layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	m_layoutBinding.descriptorCount = 1;
	m_layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	m_layoutBinding.pImmutableSamplers = NULL;

	m_descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	m_descriptorLayout.pNext = NULL;
	m_descriptorLayout.bindingCount = 1;
	m_descriptorLayout.pBindings = &m_layoutBinding;
}

vikaPipeline::~vikaPipeline()
{
	destroy();
}

// count of descriptorsets needs to be same in multiple places
bool vikaPipeline::create(uint32_t descriptorSetCount)
{
	m_layouts.resize(descriptorSetCount);
	m_res = vkCreateDescriptorSetLayout(m_logDevice->getDevice(), &m_descriptorLayout, NULL, m_layouts.data());
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

    m_pipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    m_pipelineInfo.pNext = NULL;
    m_pipelineInfo.pushConstantRangeCount = 0;
    m_pipelineInfo.pPushConstantRanges = NULL;
    m_pipelineInfo.setLayoutCount = descriptorSetCount;
    m_pipelineInfo.pSetLayouts = m_layouts.data();

	m_res = vkCreatePipelineLayout(m_logDevice->getDevice(), &m_pipelineInfo, NULL, &m_pipelineLayout);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

void vikaPipeline::destroy()
{
	if (m_pipelineLayout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(m_logDevice->getDevice(), m_pipelineLayout, NULL);
		m_pipelineLayout = VK_NULL_HANDLE;
	}

	for (size_t i = 0; i < m_layouts.size(); i++)
	{
		vkDestroyDescriptorSetLayout(m_logDevice->getDevice(), m_layouts[i], NULL);
	}
	m_layouts.clear();
}

