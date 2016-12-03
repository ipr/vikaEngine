// vikaImage : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaImage.h"
#include "vikaDevice.h"
#include "vikaBuffer.h"

#include <vulkan/vulkan.h>

vikaImage::vikaImage(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_devMemory(nullptr),
	m_image(VK_NULL_HANDLE)
{
	m_imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	m_imageInfo.pNext = NULL;
	m_imageInfo.imageType = VK_IMAGE_TYPE_2D;
	//m_imageInfo.format = depth_format; // filled in later
	//m_imageInfo.extent.width = imageSize.width;
	//m_imageInfo.extent.height = imageSize.height;
	m_imageInfo.extent.depth = 1;
	m_imageInfo.mipLevels = 1;
	m_imageInfo.arrayLayers = 1;
	//m_imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	//m_imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
	m_imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	m_imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	m_imageInfo.queueFamilyIndexCount = 0;
	m_imageInfo.pQueueFamilyIndices = NULL;
	m_imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	m_imageInfo.flags = 0;

	m_subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	m_subResource.mipLevel = 0;
	m_subResource.arrayLayer = 0;

}

vikaImage::~vikaImage()
{
	destroy();
}

// Number of samples needs to be the same at image creation,
// renderpass creation and pipeline creation.
//
bool vikaImage::create()
{
	/* expect caller to set these and other necessary stuff first
	m_imageInfo.format = format;
    m_imageInfo.samples = sampleCount;
    m_imageInfo.extent.width = imageSize.width;
    m_imageInfo.extent.height = imageSize.height;
	*/

    m_res = vkCreateImage(m_logDevice->getDevice(), &m_imageInfo, NULL, &m_image);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	vkGetImageMemoryRequirements(m_logDevice->getDevice(), m_image, &m_memReqs);
	return true;
}

void vikaImage::destroy()
{
	if (m_image != VK_NULL_HANDLE)
	{
		vkDestroyImage(m_logDevice->getDevice(), m_image, NULL);
		m_image = VK_NULL_HANDLE;
	}
}

bool vikaImage::bindToMemory(vikaDevMemory *devMemory)
{
	m_res = vkBindImageMemory(m_logDevice->getDevice(), m_image, devMemory->m_devMemory, 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	m_devMemory = devMemory;
	return true;
}

void vikaImage::getResourceLayout()
{
	vkGetImageSubresourceLayout(m_logDevice->getDevice(), m_image, &m_subResource, &m_layout);
}
