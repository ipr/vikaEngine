// vikaSampler : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSampler.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaSampler::vikaSampler(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_sampler(VK_NULL_HANDLE)
{
	m_samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	m_samplerInfo.magFilter = VK_FILTER_NEAREST;
	m_samplerInfo.minFilter = VK_FILTER_NEAREST;
	m_samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	m_samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	m_samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	m_samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	m_samplerInfo.mipLodBias = 0.0;
	m_samplerInfo.anisotropyEnable = VK_FALSE,
	m_samplerInfo.maxAnisotropy = 0;
	m_samplerInfo.compareEnable = VK_FALSE;
	m_samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
	m_samplerInfo.minLod = 0.0;
	m_samplerInfo.maxLod = 0.0;
	m_samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
}

vikaSampler::~vikaSampler()
{
	destroy();
}

bool vikaSampler::create()
{
	m_res = vkCreateSampler(m_logDevice->getDevice(), &m_samplerInfo, NULL, &m_sampler);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

void vikaSampler::destroy()
{
	if (m_sampler != VK_NULL_HANDLE)
	{
		vkDestroySampler(m_logDevice->getDevice(), m_sampler, NULL);
		m_sampler = VK_NULL_HANDLE;
	}
}

