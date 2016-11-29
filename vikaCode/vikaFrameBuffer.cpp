// vikaFrameBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaFrameBuffer.h"
#include "vikaDevice.h"
#include "vikaRenderPass.h"
#include "vikaDepthBuffer.h"
#include "vikaSwapChain.h"
#include "vikaCommandBuffer.h"

#include <vulkan/vulkan.h>

vikaFrameBuffer::vikaFrameBuffer(vikaDevice *logDevice, vikaDepthBuffer *depthBuffer, vikaSwapChain *swapchain, vikaCommandBuffer *commandBuffer) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_depthBuffer(depthBuffer),
	m_swapchain(swapchain),
	m_commandBuffer(commandBuffer)
{
	m_attachments.resize(2);

	m_bufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	m_bufferInfo.pNext = NULL;
	m_bufferInfo.flags = 0;
	//m_bufferInfo.renderPass = m_renderPass->m_renderpass; // filled in later
	//m_bufferInfo.attachmentCount = m_attachments.size();
	//m_bufferInfo.pAttachments = m_attachments.data();
	//m_bufferInfo.height = imageSize.height;
	//m_bufferInfo.width = imageSize.width;
	m_bufferInfo.layers = 1;
}

vikaFrameBuffer::~vikaFrameBuffer()
{
	destroy();
}

bool vikaFrameBuffer::create(vikaRenderPass *renderPass, VkExtent2D &imageSize)
{
    m_attachments[1] = m_depthBuffer->m_view;
	m_bufferInfo.height = imageSize.height;
	m_bufferInfo.width = imageSize.width;
	m_bufferInfo.renderPass = renderPass->m_renderpass;
	m_bufferInfo.attachmentCount = m_attachments.size();
	m_bufferInfo.pAttachments = m_attachments.data();

	m_frameBuffers.resize(m_swapchain->m_swapchainImageCount);
	for (uint32_t i = 0; i < m_swapchain->m_swapchainImageCount; i++)
	{
		m_attachments[0] = m_swapchain->m_swapchainViews[i];
		m_res = vkCreateFramebuffer(m_logDevice->getDevice(), &m_bufferInfo, NULL, &m_frameBuffers[i]);
		if (m_res != VK_SUCCESS)
		{
			return false;
		}
	}
	return true;
}

void vikaFrameBuffer::destroy()
{
	for (size_t i = 0; i < m_frameBuffers.size(); i++)
	{
		vkDestroyFramebuffer(m_logDevice->getDevice(), m_frameBuffers[i], NULL);
	}
	m_frameBuffers.clear();
}

void vikaFrameBuffer::setViewports(uint32_t viewportCount)
{
	VkViewport viewport;

	viewport.height = (float)m_bufferInfo.height;
	viewport.width = (float)m_bufferInfo.width;
	viewport.minDepth = (float)0.0f;
	viewport.maxDepth = (float)1.0f;
	viewport.x = 0;
	viewport.y = 0;

	vkCmdSetViewport(m_commandBuffer->getCmd(0), 0, viewportCount, &viewport);
}

void vikaFrameBuffer::setScissors(uint32_t scissorsCount)
{
	VkRect2D scissorRect;

	scissorRect.extent.width = m_bufferInfo.width;
	scissorRect.extent.height = m_bufferInfo.height;
	scissorRect.offset.x = 0;
	scissorRect.offset.y = 0;

	vkCmdSetScissor(m_commandBuffer->getCmd(0), 0, scissorsCount, &scissorRect);
}
