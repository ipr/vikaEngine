// vikaUniformBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaUniformBuffer.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaUniformBuffer::vikaUniformBuffer(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_bufferSize(0),
	m_buffer(VK_NULL_HANDLE)
{
    m_bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    m_bufferInfo.pNext = NULL;
    m_bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_bufferInfo.size = m_bufferSize;
    m_bufferInfo.queueFamilyIndexCount = 0;
    m_bufferInfo.pQueueFamilyIndices = NULL;
    m_bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    m_bufferInfo.flags = 0;
}

vikaUniformBuffer::~vikaUniformBuffer()
{
	destroy();
}

// structures not finished yet.. plenty of parameters
bool vikaUniformBuffer::create()
{
    m_res = vkCreateBuffer(m_logDevice->getDevice(), &m_bufferInfo, NULL, &m_buffer);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

void vikaUniformBuffer::destroy()
{
	if (m_buffer != VK_NULL_HANDLE)
	{
	    vkDestroyBuffer(m_logDevice->getDevice(), m_buffer, NULL);
		m_buffer = VK_NULL_HANDLE;
	}
}

