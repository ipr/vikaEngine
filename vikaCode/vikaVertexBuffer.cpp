// vikaVertexBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaVertexBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"
#include "vikaCommandBuffer.h"
//#include "vikaSemaphore.h"

#include <vulkan/vulkan.h>

vikaVertexBuffer::vikaVertexBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, vikaCommandBuffer *commandBuffer) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_commandBuffer(commandBuffer),
	//m_semaphore(nullptr),
	m_buffer(VK_NULL_HANDLE),
	m_devMemory(VK_NULL_HANDLE)
{
	m_bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	m_bufferInfo.pNext = NULL;
	m_bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	m_bufferInfo.size = 0; 
	m_bufferInfo.queueFamilyIndexCount = 0;
	m_bufferInfo.pQueueFamilyIndices = NULL;
	m_bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	m_bufferInfo.flags = 0;

	m_memInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	m_memInfo.pNext = NULL;
	m_memInfo.memoryTypeIndex = 0;

	//m_viBindings.resize(1);
	//m_viAttribs.resize(1);
}

vikaVertexBuffer::~vikaVertexBuffer()
{
	destroy();
}

// buffersize: total vertex data
// stridesize: single element size in the buffer
bool vikaVertexBuffer::create(VkDeviceSize bufferSize, uint32_t strideSize)
{
	m_bufferInfo.size = bufferSize; 

	m_viBindings.resize(1);
	m_viBindings[0].binding = 0;
	m_viBindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	m_viBindings[0].stride = strideSize;

	m_viAttribs.resize(2);
    m_viAttribs[0].binding = 0;
    m_viAttribs[0].location = 0;
    m_viAttribs[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    m_viAttribs[0].offset = 0;
    m_viAttribs[1].binding = 0;
    m_viAttribs[1].location = 1;
    m_viAttribs[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    m_viAttribs[1].offset = 16;

	m_clearValues.resize(2);
	m_clearValues[0].color.float32[0] = 0.2f;
	m_clearValues[0].color.float32[1] = 0.2f;
	m_clearValues[0].color.float32[2] = 0.2f;
	m_clearValues[0].color.float32[3] = 0.2f;
	m_clearValues[1].depthStencil.depth = 1.0f;
	m_clearValues[1].depthStencil.stencil = 0;

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

	//VkClearValue..

	// TODO:
	//m_renderPass->beginPass(VK_SUBPASS_CONTENTS_INLINE);

	/*
    vkCmdBindVertexBuffers(info.cmd, 0,             // Start Binding 
                           1,                       // Binding Count 
                           &info.vertex_buffer.buf, // pBuffers 
                           offsets);                // pOffsets 
						   */

	//m_renderPass->endPass();
	//return true;
}

void vikaVertexBuffer::destroy()
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

// parameter expected: vertex data and size of it
bool vikaVertexBuffer::copyToMemory(uint32_t sizeVertices, void *dataVertices)
{
	uint8_t *pMapping = nullptr; // pointer to mapping in user space (actually in device memory)
	m_res = vkMapMemory(m_logDevice->getDevice(), m_devMemory, 0, m_memReqs.size, 0, (void**)&pMapping);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	// copy to device via our mapping
	memcpy(pMapping, dataVertices, sizeVertices);
	vkUnmapMemory(m_logDevice->getDevice(), m_devMemory);

	m_res = vkBindBufferMemory(m_logDevice->getDevice(), m_buffer, m_devMemory, 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

