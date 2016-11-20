// vikaCommandBuffer : manage pool and buffers for commands
//
// vulkan pool should handle most of the work, define only wrapper to help with API
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaCommandBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"

#include <vulkan/vulkan.h>

vikaCommandBuffer::vikaCommandBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_cmdPool(VK_NULL_HANDLE)
{
	m_cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	m_cmdPoolInfo.pNext = NULL;
	//m_cmdPoolInfo.queueFamilyIndex = m_physDevice->getQueueIndex();
	m_cmdPoolInfo.flags = 0;

	m_cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_cmdBufferInfo.pNext = NULL;
	//m_cmdBufferInfo.commandPool = m_cmdPool; // fill in later
	m_cmdBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	//m_cmdBufferInfo.commandBufferCount = bufferCount; // fill in later

}

vikaCommandBuffer::~vikaCommandBuffer()
{
	destroy();
}

bool vikaCommandBuffer::create(uint32_t bufferCount)
{
	m_cmdPoolInfo.queueFamilyIndex = m_physDevice->getQueueIndex();

	m_cmdBuffers.resize(bufferCount);
	m_res = vkCreateCommandPool(m_logDevice->getDevice(), &m_cmdPoolInfo, NULL, &m_cmdPool);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_cmdBufferInfo.commandPool = m_cmdPool;
	m_cmdBufferInfo.commandBufferCount = bufferCount;

	m_res = vkAllocateCommandBuffers(m_logDevice->getDevice(), &m_cmdBufferInfo, m_cmdBuffers.data());
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaCommandBuffer::destroy()
{
	if (m_cmdBuffers.size() > 0)
	{
		vkFreeCommandBuffers(m_logDevice->getDevice(), m_cmdPool, m_cmdBufferInfo.commandBufferCount, m_cmdBuffers.data());
		m_cmdBuffers.clear();
	}
	if (m_cmdPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(m_logDevice->getDevice(), m_cmdPool, NULL);
		m_cmdPool = VK_NULL_HANDLE;
	}
}

bool vikaCommandBuffer::resetPool()
{
	//m_res = vkResetCommandPool(device, m_cmdPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	m_res = vkResetCommandPool(m_logDevice->getDevice(), m_cmdPool, 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

bool vikaCommandBuffer::resetBuffer(uint32_t bufferIndex)
{
	//m_res = vkResetCommandBuffer(m_cmdBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	m_res = vkResetCommandBuffer(m_cmdBuffers[bufferIndex], 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

bool vikaCommandBuffer::executeEnd(uint32_t bufferIndex)
{
	m_res = vkEndCommandBuffer(m_cmdBuffers[bufferIndex]);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

