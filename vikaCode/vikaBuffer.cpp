// vikaBuffer : base buffer for various cases
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"

#include <vulkan/vulkan.h>

vikaDevMemory::vikaDevMemory(vikaDevice *logDevice, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_isMapped(false),
	m_devMemory(VK_NULL_HANDLE)
{
	m_memInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	m_memInfo.pNext = NULL;
	//m_memInfo.allocationSize = 0; // filled in later
	m_memInfo.memoryTypeIndex = 0;
}

vikaDevMemory::~vikaDevMemory()
{
	destroy();
}

bool vikaDevMemory::create(VkDeviceSize bufferSize)
{
	m_memInfo.allocationSize = bufferSize;

	m_res = vkAllocateMemory(m_logDevice->getDevice(), &m_memInfo, NULL, &m_devMemory);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaDevMemory::destroy()
{
	if (m_devMemory != VK_NULL_HANDLE)
	{
		if (m_isMapped == true)
		{
			unmapMem();
		}
		vkFreeMemory(m_logDevice->getDevice(), m_devMemory, NULL);
		m_devMemory = VK_NULL_HANDLE;
	}
}

uint8_t *vikaDevMemory::mapMem(VkDeviceSize size, VkDeviceSize offset)
{
	uint8_t *pMapping = nullptr; // pointer to mapping in user space (actually in device memory)
	m_res = vkMapMemory(m_logDevice->getDevice(), m_devMemory, offset, size, 0, (void**)&pMapping);
	if (m_res == VK_SUCCESS)
	{
		m_isMapped = true;
		return pMapping;
	}
	return nullptr;
}

void vikaDevMemory::unmapMem()
{
	vkUnmapMemory(m_logDevice->getDevice(), m_devMemory);
	m_isMapped = false;
}

//////////////////

vikaBuffer::vikaBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, VkBufferUsageFlags usage) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_devMemory(nullptr),
	m_buffer(VK_NULL_HANDLE)
{
	m_bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	m_bufferInfo.pNext = NULL;
	m_bufferInfo.usage = usage;
	m_bufferInfo.size = 0; 
	m_bufferInfo.queueFamilyIndexCount = 0;
	m_bufferInfo.pQueueFamilyIndices = NULL;
	m_bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	m_bufferInfo.flags = 0;

	m_devMemory = new vikaDevMemory(logDevice, physDevice);
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
	if (m_physDevice->memtypeBitsToIndex(reqMask, m_memReqs.memoryTypeBits, 
									m_devMemory->m_memInfo.memoryTypeIndex) == false)
	{
		return false;
	}

	if (m_devMemory->create(m_memReqs.size) == false)
	{
		return false;
	}
	return true;
}

void vikaBuffer::destroy()
{
	if (m_devMemory != nullptr)
	{
		m_devMemory->destroy();
		delete m_devMemory;
		m_devMemory = nullptr;
	}

	if (m_buffer != VK_NULL_HANDLE)
	{
	    vkDestroyBuffer(m_logDevice->getDevice(), m_buffer, NULL);
		m_buffer = VK_NULL_HANDLE;
	}
}

bool vikaBuffer::copyToMemory(uint32_t size, void *data)
{
	uint8_t *pMapping = m_devMemory->mapMem(m_memReqs.size);
	if (pMapping == nullptr)
	{
		return false;
	}

	// copy to device via our mapping
	memcpy(pMapping, data, size);
	m_devMemory->unmapMem();

	m_res = vkBindBufferMemory(m_logDevice->getDevice(), m_buffer, m_devMemory->m_devMemory, 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

