// vikaSampler : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSampler.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"
#include "vikaBuffer.h"
#include "vikaImage.h"

#include <vulkan/vulkan.h>

vikaSampler::vikaSampler(vikaDevice *logDevice, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_devMemory(nullptr),
	m_image(nullptr),
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
	// TODO: determine these
	bool needStaging = true;
	VkExtent2D texSize;

	m_image = new vikaImage(m_logDevice);
	m_image->m_imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
	m_image->m_imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    m_image->m_imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    m_image->m_imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
	m_image->m_imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	if (needStaging == false)
	{
		m_image->m_imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
	}

	// TODO:
    //m_image->m_imageInfo.extent.width = texSize.width;
    //m_image->m_imageInfo.extent.height = texSize.height;

	if (m_image->create() == false)
	{
		return false;
	}
    //vkGetImageMemoryRequirements(m_logDevice->getDevice(), m_image->m_image, &m_image->m_memReqs);

	m_devMemory = new vikaDevMemory(m_logDevice, m_physDevice);
	if (m_physDevice->memtypeBitsToIndex(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
										m_image->m_memReqs.memoryTypeBits, 
										m_devMemory->m_memInfo.memoryTypeIndex) == false)
	{
		return false;
	}
	if (m_devMemory->create(m_image->m_memReqs.size) == false)
	{
		return false;
	}

	if (m_image->bindToMemory(m_devMemory) == false)
	{
		return false;
	}
	m_image->getResourceLayout(); // 

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

	if (m_image != nullptr)
	{
		m_image->destroy();
		delete m_image;
		m_image = nullptr;
	}

	if (m_devMemory != nullptr)
	{
		m_devMemory->destroy();
		delete m_devMemory;
		m_devMemory = nullptr;
	}
}

