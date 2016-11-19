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
	m_bufferSize(0), // TODO: size needed
	m_buffer(VK_NULL_HANDLE),
	m_devMemory(VK_NULL_HANDLE)
{
    m_bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    m_bufferInfo.pNext = NULL;
    m_bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_bufferInfo.size = m_bufferSize;
    m_bufferInfo.queueFamilyIndexCount = 0;
    m_bufferInfo.pQueueFamilyIndices = NULL;
    m_bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    m_bufferInfo.flags = 0;

	m_memInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    m_memInfo.pNext = NULL;
    m_memInfo.memoryTypeIndex = 0;
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

	vkGetBufferMemoryRequirements(m_logDevice->getDevice(), m_buffer, &m_memReqs);
    m_memInfo.allocationSize = m_memReqs.size;

	// TODO: memory type stuff to m_memInfo

	m_res = vkAllocateMemory(m_logDevice->getDevice(), &m_memInfo, NULL, &m_devMemory);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	//m_res = vkMapMemory();
	//vkUnmapMemory();

	//m_res = vkBindBufferMemory();

	return true;
}

void vikaUniformBuffer::destroy()
{
	if (m_devMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(m_logDevice->getDevice(), m_devMemory, NULL);
		m_devMemory = VK_NULL_HANDLE;
	}

	if (m_buffer != VK_NULL_HANDLE)
	{
	    vkDestroyBuffer(m_logDevice->getDevice(), m_buffer, NULL);
		m_buffer = VK_NULL_HANDLE;
	}
}

