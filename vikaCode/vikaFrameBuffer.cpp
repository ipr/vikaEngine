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
	m_commandBuffer->setViewports(m_bufferInfo.width, m_bufferInfo.height, viewportCount, 0);
}

void vikaFrameBuffer::setScissors(uint32_t scissorsCount)
{
	m_commandBuffer->setScissors(m_bufferInfo.width, m_bufferInfo.height, scissorsCount, 0);
}

