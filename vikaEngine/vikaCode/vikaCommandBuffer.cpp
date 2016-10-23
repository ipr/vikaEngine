// vikaCommandBuffer : manage pool and buffers for commands
//
// vulkan pool should handle most of the work, define only wrapper to help with API
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaCommandBuffer.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>


vikaCommandBuffer::vikaCommandBuffer(vikaDevice *parent, const uint32_t queueIndex, uint32_t bufferCount) :
	m_res(VK_SUCCESS),
	m_parent(parent),
	m_queueIndex(queueIndex),
	m_bufferCount(bufferCount),
	m_cmdPool(VK_NULL_HANDLE)
{
}

vikaCommandBuffer::~vikaCommandBuffer()
{
	destroy();
}

bool vikaCommandBuffer::create()
{
	m_cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	m_cmdPoolInfo.pNext = NULL;
	m_cmdPoolInfo.queueFamilyIndex = m_queueIndex;
	m_cmdPoolInfo.flags = 0;

	m_res = vkCreateCommandPool(m_parent->getDevice(), &m_cmdPoolInfo, NULL, &m_cmdPool);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_cmdBufferInfo.pNext = NULL;
	m_cmdBufferInfo.commandPool = m_cmdPool;
	m_cmdBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_cmdBufferInfo.commandBufferCount = m_bufferCount;

	m_cmdBuffers.resize(m_bufferCount);
	m_res = vkAllocateCommandBuffers(m_parent->getDevice(), &m_cmdBufferInfo, m_cmdBuffers.data());
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaCommandBuffer::destroy()
{
	if (m_cmdBuffers.empty() == false)
	{
		vkFreeCommandBuffers(m_parent->getDevice(), m_cmdPool, m_cmdBufferInfo.commandBufferCount, m_cmdBuffers.data());
		m_cmdBuffers.clear();
	}
	if (m_cmdPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(m_parent->getDevice(), m_cmdPool, NULL);
		m_cmdPool = VK_NULL_HANDLE;
	}
}

bool vikaCommandBuffer::resetPool()
{
	//m_res = vkResetCommandPool(device, m_cmdPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	m_res = vkResetCommandPool(m_parent->getDevice(), m_cmdPool, 0);
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

