// vikaDepthBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaDepthBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"

#include <vulkan/vulkan.h>

vikaDepthBuffer::vikaDepthBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_image(VK_NULL_HANDLE),
	m_view(VK_NULL_HANDLE),
	m_devMemory(VK_NULL_HANDLE)
{
	m_memInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	m_memInfo.pNext = NULL;
    m_memInfo.allocationSize = 0;
    m_memInfo.memoryTypeIndex = 0;

    m_imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    m_imageInfo.pNext = NULL;
}

vikaDepthBuffer::~vikaDepthBuffer()
{
	destroy();
}

bool vikaDepthBuffer::create()
{
	vkGetPhysicalDeviceFormatProperties(m_physDevice->getPhysDev(), VK_FORMAT_D16_UNORM, &m_formatProp);
	if (m_formatProp.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
	{
		m_imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
	}
	else if (m_formatProp.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
	{
		m_imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	}
	else
	{
		// or try other formats..
		return false;
	}

    m_res = vkCreateImage(m_logDevice->getDevice(), &m_imageInfo, NULL, &m_image);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	vkGetImageMemoryRequirements(m_logDevice->getDevice(), m_image, &m_memReqs);

	// no flags..
	// use m_memReqs.memoryTypeBits to get m_memInfo.memoryTypeIndex
	if (memtypeBitsToIndex() == false)
	{
		return false;
	}

	m_res = vkAllocateMemory(m_logDevice->getDevice(), &m_memInfo, NULL, &m_devMemory);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_res = vkBindImageMemory(m_logDevice->getDevice(), m_image, m_devMemory, 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_res = vkCreateImageView(m_logDevice->getDevice(), &m_viewInfo, NULL, &m_view);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

void vikaDepthBuffer::destroy()
{
	if (m_view != VK_NULL_HANDLE)
	{
	    vkDestroyImageView(m_logDevice->getDevice(), m_view, NULL);
		m_view = VK_NULL_HANDLE;
	}

	if (m_image != VK_NULL_HANDLE)
	{
		vkDestroyImage(m_logDevice->getDevice(), m_image, NULL);
		m_image = VK_NULL_HANDLE;
	}

	if (m_devMemory != VK_NULL_HANDLE)
	{
	    vkFreeMemory(m_logDevice->getDevice(), m_devMemory, NULL);
		m_devMemory = VK_NULL_HANDLE;
	}
}

// this method is pretty directly from the API-sample under Apache license
bool vikaDepthBuffer::memtypeBitsToIndex(VkFlags reqMask)
{
	// handle this one here too
	m_memInfo.allocationSize = m_memReqs.size;

	uint32_t memoryTypeBits = m_memReqs.memoryTypeBits;
	for (uint32_t i = 0; i < m_physDevice->m_memoryProperties.memoryTypeCount; i++)
	{
		if ((memoryTypeBits & 1) == 1)
		{
			if ((m_physDevice->m_memoryProperties.memoryTypes[i].propertyFlags & reqMask) == reqMask)
			{
				m_memInfo.memoryTypeIndex = i;
				return true;
			}
		}
		memoryTypeBits >>= 1;
	}
	return false;
}
