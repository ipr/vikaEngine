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
	//m_cmdPoolInfo.queueFamilyIndex = m_physDevice->m_graphicsQueueIndex;
	m_cmdPoolInfo.flags = 0;

	m_cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_cmdBufferInfo.pNext = NULL;
	//m_cmdBufferInfo.commandPool = m_cmdPool; // fill in later
	m_cmdBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	//m_cmdBufferInfo.commandBufferCount = bufferCount; // fill in later

	m_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	m_beginInfo.pNext = NULL;
	m_beginInfo.flags = 0;
	m_beginInfo.pInheritanceInfo = NULL;
}

vikaCommandBuffer::~vikaCommandBuffer()
{
	destroy();
}

bool vikaCommandBuffer::create(uint32_t graphicsQueueIndex, uint32_t bufferCount)
{
	m_cmdPoolInfo.queueFamilyIndex = graphicsQueueIndex;

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

bool vikaCommandBuffer::executeBegin(uint32_t bufferIndex)
{
	m_res = vkBeginCommandBuffer(m_cmdBuffers[bufferIndex], &m_beginInfo);
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

bool vikaCommandBuffer::executeQueue(uint32_t bufferIndex)
{
	VkFence drawFence(VK_NULL_HANDLE);
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.pNext = NULL;
	fenceInfo.flags = 0;
    m_res = vkCreateFence(m_logDevice->getDevice(), &fenceInfo, NULL, &drawFence);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	VkPipelineStageFlags pipeFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = NULL;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = NULL;
    submitInfo.pWaitDstStageMask = &pipeFlags;
    submitInfo.commandBufferCount = m_cmdBuffers.size(); // TODO: does this work if count > 1 ?
    submitInfo.pCommandBuffers = m_cmdBuffers.data();
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = NULL;

	VkQueue &gfxQueue = m_logDevice->getGraphicsQueue();
	m_res = vkQueueSubmit(gfxQueue, 1, &submitInfo, drawFence);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	/* Amount of time, in nanoseconds, to wait for a command buffer to complete */
	uint64_t fenceTimeout = 100000000;
	do 
	{
		m_res = vkWaitForFences(m_logDevice->getDevice(), 1, &drawFence, VK_TRUE, fenceTimeout);
	} while (m_res == VK_TIMEOUT);

    vkDestroyFence(m_logDevice->getDevice(), drawFence, NULL);
	return true;
}
