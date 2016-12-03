// vikaCommandBuffer : manage pool and buffers for commands
//
// vulkan pool should handle most of the work, define only wrapper to help with API
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaCommandBuffer.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaCommandBuffer::vikaCommandBuffer(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
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

bool vikaCommandBuffer::executeQueue(vikaFence &fence)
{
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

	m_res = vkQueueSubmit(m_logDevice->getGraphicsQueue(), 1, &submitInfo, fence.getFence());
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaCommandBuffer::commandDraw(uint32_t verticesCount, uint32_t bufferIndex)
{
	vkCmdDraw(m_cmdBuffers[bufferIndex], 
				verticesCount, // vertex count
				1, // instance count
				0,  // first vertex
				0); // first instance
}

void vikaCommandBuffer::commandLineWidth(float lineWidth, uint32_t bufferIndex)
{
	vkCmdSetLineWidth(m_cmdBuffers[bufferIndex], lineWidth);
}

void vikaCommandBuffer::commandDepthBias(float cFactor, float clamp, float slope, uint32_t bufferIndex)
{
	vkCmdSetDepthBias(m_cmdBuffers[bufferIndex], cFactor, clamp, slope);
}

void vikaCommandBuffer::setViewports(uint32_t width, uint32_t height, uint32_t viewportCount, uint32_t bufferIndex)
{
	m_viewports.resize(viewportCount);

	VkViewport &viewport = m_viewports.back();
	viewport.width = (float)width;
	viewport.height = (float)height;
	viewport.minDepth = (float)0.0f;
	viewport.maxDepth = (float)1.0f;
	viewport.x = 0;
	viewport.y = 0;

	vkCmdSetViewport(m_cmdBuffers[bufferIndex], 0, m_viewports.size(), m_viewports.data());
}

void vikaCommandBuffer::setScissors(uint32_t width, uint32_t height, uint32_t scissorsCount, uint32_t bufferIndex)
{
	m_scissors.resize(scissorsCount);

	VkRect2D &scissorRect = m_scissors.back();
	scissorRect.extent.width = width;
	scissorRect.extent.height = height;
	scissorRect.offset.x = 0;
	scissorRect.offset.y = 0;

	vkCmdSetScissor(m_cmdBuffers[bufferIndex], 0, m_scissors.size(), m_scissors.data());
}

void vikaCommandBuffer::setBlendFactors(float constants[4], uint32_t bufferIndex)
{
	//float blendConstants[4] = {0};
	vkCmdSetBlendConstants(m_cmdBuffers[bufferIndex], constants);
}

void vikaCommandBuffer::setDepthBounds(float min, float max, uint32_t bufferIndex)
{
	vkCmdSetDepthBounds(m_cmdBuffers[bufferIndex], min, max);
}

void vikaCommandBuffer::setStencilCompareMask(uint32_t bufferIndex)
{
	/*
	VkStencilFaceFlags flags;

	vkCmdSetStencilCompareMask(m_cmdBuffers[bufferIndex]);
	*/
}

void vikaCommandBuffer::setStencilWriteMask(uint32_t bufferIndex)
{
	/*
	VkStencilFaceFlags flags;

	vkCmdSetStencilWriteMask(m_cmdBuffers[bufferIndex]);
	*/
}

void vikaCommandBuffer::setStencilReference(uint32_t bufferIndex)
{
	/*
	VkStencilFaceFlags flags;

	vkCmdSetStencilReference(m_cmdBuffers[bufferIndex]);
	*/
}
