// vikaPipeline : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaPipeline.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaPipeline::vikaPipeline(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice)
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

// structures not finished yet.. plenty of parameters
bool vikaPipeline::create()
{
	return true;
}

void vikaPipeline::destroy()
{
}

