// vikaUniformBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaUniformBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"

#include <vulkan/vulkan.h>

vikaUniformBuffer::vikaUniformBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_buffer(VK_NULL_HANDLE),
	m_devMemory(VK_NULL_HANDLE)
{
	m_bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	m_bufferInfo.pNext = NULL;
	m_bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	m_bufferInfo.size = 0; 
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

bool vikaUniformBuffer::create(VkDeviceSize bufferSize)
{
	m_bufferInfo.size = bufferSize; 

    m_res = vkCreateBuffer(m_logDevice->getDevice(), &m_bufferInfo, NULL, &m_buffer);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_descInfo.buffer = m_buffer;
	m_descInfo.offset = 0;
	m_descInfo.range = m_bufferInfo.size;

	vkGetBufferMemoryRequirements(m_logDevice->getDevice(), m_buffer, &m_memReqs);

	// use m_memReqs.memoryTypeBits to get m_memInfo.memoryTypeIndex
	m_memInfo.allocationSize = m_memReqs.size;
	if (m_physDevice->memtypeBitsToIndex(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
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

// parameter expected: mat4 with view-projection matrix
bool vikaUniformBuffer::copyToMemory(uint32_t sizeMVP, void *dataMVP)
{
	// following step is used to copy current projection-view matrix into allocated buffer

	uint8_t *pMapping = nullptr; // pointer to mapping in user space (actually in device memory)
	m_res = vkMapMemory(m_logDevice->getDevice(), m_devMemory, 0, m_memReqs.size, 0, (void**)&pMapping);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	// copy to device via our mapping
	memcpy(pMapping, dataMVP, sizeMVP);
	vkUnmapMemory(m_logDevice->getDevice(), m_devMemory);

	m_res = vkBindBufferMemory(m_logDevice->getDevice(), m_buffer, m_devMemory, 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}
