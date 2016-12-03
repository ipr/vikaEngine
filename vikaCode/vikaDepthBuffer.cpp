// vikaDepthBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaDepthBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"
//#include "vikaBuffer.h"
#include "vikaImage.h"

#include <vulkan/vulkan.h>

vikaDepthBuffer::vikaDepthBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_depthFormat(VK_FORMAT_D16_UNORM),
	m_image(nullptr),
	//m_image(VK_NULL_HANDLE),
	m_view(VK_NULL_HANDLE),
	m_devMemory(VK_NULL_HANDLE)
{
	m_image = new vikaImage(logDevice);

	m_memInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	m_memInfo.pNext = NULL;
    m_memInfo.allocationSize = 0;
    m_memInfo.memoryTypeIndex = 0;

	m_viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	m_viewInfo.pNext = NULL;
    m_viewInfo.image = VK_NULL_HANDLE;
    //m_viewInfo.format = depth_format; // filled in later
    m_viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    m_viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    m_viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    m_viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    m_viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    m_viewInfo.subresourceRange.baseMipLevel = 0;
    m_viewInfo.subresourceRange.levelCount = 1;
    m_viewInfo.subresourceRange.baseArrayLayer = 0;
    m_viewInfo.subresourceRange.layerCount = 1;
    m_viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    m_viewInfo.flags = 0;
}

vikaDepthBuffer::~vikaDepthBuffer()
{
	destroy();
}

// Number of samples needs to be the same at image creation,
// renderpass creation and pipeline creation.
//
bool vikaDepthBuffer::create(VkExtent2D &imageSize, VkSampleCountFlagBits sampleCount, VkFormat depthFormat)
{
	// TODO: try other formats as well
	//VkFormat depthFormat = VK_FORMAT_D16_UNORM;
	VkImageTiling tiling = VK_IMAGE_TILING_LINEAR;
	vkGetPhysicalDeviceFormatProperties(m_physDevice->getPhysDev(), depthFormat, &m_formatProp);
	if (m_formatProp.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
	{
		tiling = VK_IMAGE_TILING_LINEAR;
	}
	else if (m_formatProp.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
	{
		tiling = VK_IMAGE_TILING_OPTIMAL;
	}
	else
	{
		// or try other formats..
		return false;
	}
	m_depthFormat = depthFormat;
	m_viewInfo.format = depthFormat;

	m_image->m_imageInfo.tiling = tiling;
	m_image->m_imageInfo.format = depthFormat;
    m_image->m_imageInfo.samples = sampleCount;
    m_image->m_imageInfo.extent.width = imageSize.width;
    m_image->m_imageInfo.extent.height = imageSize.height;
	if (m_image->create() == false)
	{
		return false;
	}

	// no flags..
	// use m_memReqs.memoryTypeBits to get m_memInfo.memoryTypeIndex
	m_memInfo.allocationSize = m_image->m_memReqs.size;
	if (m_physDevice->memtypeBitsToIndex(0, m_image->m_memReqs.memoryTypeBits, m_memInfo.memoryTypeIndex) == false)
	{
		return false;
	}

	m_res = vkAllocateMemory(m_logDevice->getDevice(), &m_memInfo, NULL, &m_devMemory);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_res = vkBindImageMemory(m_logDevice->getDevice(), m_image->m_image, m_devMemory, 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	// after binding
	m_viewInfo.image = m_image->m_image;
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

	if (m_image != nullptr)
	{
		m_image->destroy();
		delete m_image;
		m_image = nullptr;
	}

	if (m_devMemory != VK_NULL_HANDLE)
	{
	    vkFreeMemory(m_logDevice->getDevice(), m_devMemory, NULL);
		m_devMemory = VK_NULL_HANDLE;
	}
}

