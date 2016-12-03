// vikaBuffer : base buffer for various cases
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"

#include <vulkan/vulkan.h>

vikaBuffer::vikaBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, VkBufferUsageFlags usage) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_buffer(VK_NULL_HANDLE),
	m_devMemory(VK_NULL_HANDLE)
{
	m_bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	m_bufferInfo.pNext = NULL;
	m_bufferInfo.usage = usage;
	m_bufferInfo.size = 0; 
	m_bufferInfo.queueFamilyIndexCount = 0;
	m_bufferInfo.pQueueFamilyIndices = NULL;
	m_bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	m_bufferInfo.flags = 0;

	m_memInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	m_memInfo.pNext = NULL;
	m_memInfo.allocationSize = 0;
	m_memInfo.memoryTypeIndex = 0;
}

vikaBuffer::~vikaBuffer()
{
	destroy();
}

// reqmask: VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
bool vikaBuffer::create(VkDeviceSize bufferSize, VkFlags reqMask)
{
	m_bufferInfo.size = bufferSize; 

    m_res = vkCreateBuffer(m_logDevice->getDevice(), &m_bufferInfo, NULL, &m_buffer);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	vkGetBufferMemoryRequirements(m_logDevice->getDevice(), m_buffer, &m_memReqs);

	// use m_memReqs.memoryTypeBits to get m_memInfo.memoryTypeIndex
	m_memInfo.allocationSize = m_memReqs.size;
	if (m_physDevice->memtypeBitsToIndex(reqMask, 
										m_memReqs.memoryTypeBits, m_memInfo.memoryTypeIndex) == false)
	{
		return false;
	}

	m_res = vkAllocateMemory(m_logDevice->getDevice(), &m_memInfo, NULL, &m_devMemory);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

void vikaBuffer::destroy()
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

bool vikaBuffer::copyToMemory(uint32_t size, void *data)
{
	uint8_t *pMapping = nullptr; // pointer to mapping in user space (actually in device memory)
	m_res = vkMapMemory(m_logDevice->getDevice(), m_devMemory, 0, m_memReqs.size, 0, (void**)&pMapping);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	// copy to device via our mapping
	memcpy(pMapping, data, size);
	vkUnmapMemory(m_logDevice->getDevice(), m_devMemory);

	m_res = vkBindBufferMemory(m_logDevice->getDevice(), m_buffer, m_devMemory, 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}
