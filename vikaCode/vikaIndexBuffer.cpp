// vikaIndexBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaIndexBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"
#include "vikaCommandBuffer.h"
//#include "vikaSemaphore.h"
#include "vikaBuffer.h"

#include <vulkan/vulkan.h>

vikaIndexBuffer::vikaIndexBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, vikaCommandBuffer *commandBuffer) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_commandBuffer(commandBuffer),
	//m_semaphore(nullptr),
	m_buffer(nullptr),
	m_indexType(VK_INDEX_TYPE_UINT32)
{
	m_buffer = new vikaBuffer(logDevice, physDevice, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
}

vikaIndexBuffer::~vikaIndexBuffer()
{
	destroy();
}

// buffersize: total vertex data
// stridesize: single element size in the buffer
bool vikaIndexBuffer::create(VkDeviceSize bufferSize, VkIndexType indexType)
{
	m_indexType = indexType;
	if (m_buffer->create(bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == false)
	{
		return false;
	}

	m_descInfo.buffer = m_buffer->m_buffer;
	m_descInfo.offset = 0;
	m_descInfo.range = m_buffer->m_bufferInfo.size;
	return true;
}

void vikaIndexBuffer::destroy()
{
	if (m_buffer != nullptr)
	{
		m_buffer->destroy();
		delete m_buffer;
		m_buffer = nullptr;
	}
}

// parameter expected: index data and size of it
bool vikaIndexBuffer::copyToMemory(uint32_t sizeIndices, void *dataIndices)
{
	return m_buffer->copyToMemory(sizeIndices, dataIndices);
}

void vikaIndexBuffer::bindIndexBuffer(uint32_t cmdBufferIndex)
{
	vkCmdBindIndexBuffer(m_commandBuffer->getCmd(cmdBufferIndex), // likely same as used in renderpass (if more than one)
						m_buffer->m_buffer,
						0, // offset
						m_indexType); // index type
}
