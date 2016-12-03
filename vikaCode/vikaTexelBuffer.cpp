// vikaTexelBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaTexelBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"
#include "vikaBuffer.h"

#include <vulkan/vulkan.h>

vikaTexelBuffer::vikaTexelBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_buffer(nullptr)
{
	m_buffer = new vikaBuffer(logDevice, physDevice, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
}

vikaTexelBuffer::~vikaTexelBuffer()
{
	destroy();
}

bool vikaTexelBuffer::create(VkDeviceSize bufferSize)
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

void vikaTexelBuffer::destroy()
{
	if (m_buffer != nullptr)
	{
		m_buffer->destroy();
		delete m_buffer;
		m_buffer = nullptr;
	}
}

bool vikaTexelBuffer::copyToMemory(uint32_t size, void *data)
{
	return m_buffer->copyToMemory(size, data);
}
