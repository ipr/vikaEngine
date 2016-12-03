// vikaUniformBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaUniformBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"
#include "vikaBuffer.h"

#include <vulkan/vulkan.h>

vikaUniformBuffer::vikaUniformBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_buffer(nullptr)
{
	m_buffer = new vikaBuffer(logDevice, physDevice, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

vikaUniformBuffer::~vikaUniformBuffer()
{
	destroy();
}

bool vikaUniformBuffer::create(VkDeviceSize bufferSize)
{
	if (m_buffer->create(bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == false)
	{
		return false;
	}

	m_descInfo.buffer = m_buffer->m_buffer;
	m_descInfo.offset = 0;
	m_descInfo.range = m_buffer->m_bufferInfo.size;

	return true;
}

void vikaUniformBuffer::destroy()
{
	if (m_buffer != nullptr)
	{
		m_buffer->destroy();
		delete m_buffer;
		m_buffer = nullptr;
	}
}

// parameter expected: mat4 with view-projection matrix
bool vikaUniformBuffer::copyToMemory(uint32_t sizeMVP, void *dataMVP)
{
	return m_buffer->copyToMemory(sizeMVP, dataMVP);
}
