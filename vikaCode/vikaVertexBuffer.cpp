// vikaVertexBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaVertexBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"
#include "vikaCommandBuffer.h"
//#include "vikaSemaphore.h"
#include "vikaBuffer.h"

#include <vulkan/vulkan.h>

vikaVertexBuffer::vikaVertexBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, vikaCommandBuffer *commandBuffer) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_commandBuffer(commandBuffer),
	//m_semaphore(nullptr),
	m_buffer(nullptr)
{
	m_buffer = new vikaBuffer(logDevice, physDevice, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

	m_viBindings.resize(1);
	m_viBindings[0].binding = 0;
	m_viBindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	//m_viBindings[0].stride = strideSize;

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
}

vikaVertexBuffer::~vikaVertexBuffer()
{
	destroy();
}

// buffersize: total vertex data
// stridesize: single element size in the buffer
bool vikaVertexBuffer::create(VkDeviceSize bufferSize, uint32_t strideSize)
{
	m_viBindings[0].stride = strideSize;

	if (m_buffer->create(bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == false)
	{
		return false;
	}

	m_descInfo.resize(1);
	m_descInfo[0].buffer = m_buffer->m_buffer;
	m_descInfo[0].offset = 0;
	m_descInfo[0].range = m_buffer->m_bufferInfo.size;
	return true;
}

void vikaVertexBuffer::destroy()
{
	if (m_buffer != nullptr)
	{
		m_buffer->destroy();
		delete m_buffer;
		m_buffer = nullptr;
	}
}

// parameter expected: vertex data and size of it
bool vikaVertexBuffer::copyToMemory(uint32_t sizeVertices, void *dataVertices)
{
	return m_buffer->copyToMemory(sizeVertices, dataVertices);
}

void vikaVertexBuffer::bindVertexBuffer(uint32_t cmdBufferIndex)
{
	// not sure what to do with this..
    const VkDeviceSize offsets[1] = {0};

	vkCmdBindVertexBuffers(m_commandBuffer->getCmd(cmdBufferIndex), // likely same as used in renderpass (if more than one)
							0,						// Start Binding 
							m_viBindings.size(),	// Binding Count 
							&m_buffer->m_buffer,	// pBuffers 
							offsets);				// pOffsets 
}

