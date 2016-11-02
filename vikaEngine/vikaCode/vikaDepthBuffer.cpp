// vikaDepthBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaDepthBuffer.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaDepthBuffer::vikaDepthBuffer(vikaDevice *parent) :
	m_res(VK_SUCCESS),
	m_parent(parent),
	m_image(VK_NULL_HANDLE)
{
    m_imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    m_imageInfo.pNext = NULL;
}

vikaDepthBuffer::~vikaDepthBuffer()
{
	destroy();
}

bool vikaDepthBuffer::create()
{
    m_res = vkCreateImage(m_parent->getDevice(), &m_imageInfo, NULL, &m_image);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaDepthBuffer::destroy()
{
	if (m_image != VK_NULL_HANDLE)
	{
		vkDestroyImage(m_parent->getDevice(), m_image, NULL);
		m_image = VK_NULL_HANDLE;
	}
}

