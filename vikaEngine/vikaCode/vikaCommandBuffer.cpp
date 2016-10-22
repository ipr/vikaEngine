// vikaCommandBuffer : manage pool and buffers for commands
//
// vulkan pool should handle most of the work, define only wrapper to help with API
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaCommandBuffer.h"

#include <vulkan/vulkan.h>


vikaCommandBuffer::vikaCommandBuffer(const uint32_t queueIndex, uint32_t bufferCount) :
	m_res(VK_SUCCESS),
	m_queueIndex(queueIndex),
	m_bufferCount(bufferCount),
	m_cmdPool(VK_NULL_HANDLE)
{
}

vikaCommandBuffer::~vikaCommandBuffer()
{
}

bool vikaCommandBuffer::create(VkDevice &device)
{
	m_cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	m_cmdPoolInfo.pNext = NULL;
	m_cmdPoolInfo.queueFamilyIndex = m_queueIndex;
	m_cmdPoolInfo.flags = 0;

	m_res = vkCreateCommandPool(device, &m_cmdPoolInfo, NULL, &m_cmdPool);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_cmdBufferInfo.pNext = NULL;
	m_cmdBufferInfo.commandPool = m_cmdPool;
	m_cmdBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_cmdBufferInfo.commandBufferCount = m_bufferCount;

	m_cmdBuffers.reserve(m_bufferCount);

	m_res = vkAllocateCommandBuffers(device, &m_cmdBufferInfo, m_cmdBuffers.data());
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_cmdBuffers.resize(m_bufferCount);
	return true;
}

void vikaCommandBuffer::destroy(VkDevice &device)
{
	if (m_cmdBuffers.size() > 0)
	{
		vkFreeCommandBuffers(device, m_cmdPool, m_cmdBufferInfo.commandBufferCount, m_cmdBuffers.data());
		m_cmdBuffers.clear();
	}
	if (m_cmdPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(device, m_cmdPool, NULL);
		m_cmdPool = VK_NULL_HANDLE;
	}
}

bool vikaCommandBuffer::resetPool(VkDevice &device)
{
	//m_res = vkResetCommandPool(device, m_cmdPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	m_res = vkResetCommandPool(device, m_cmdPool, 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

bool vikaCommandBuffer::resetBuffer(VkDevice &device, uint32_t bufferIndex)
{
	//m_res = vkResetCommandBuffer(m_cmdBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	VkCommandBuffer &cmdBuf = m_cmdBuffers[bufferIndex];
	m_res = vkResetCommandBuffer(cmdBuf, 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

