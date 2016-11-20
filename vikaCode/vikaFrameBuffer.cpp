// vikaFrameBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaFrameBuffer.h"
#include "vikaDevice.h"
#include "vikaRenderPass.h"
#include "vikaDepthBuffer.h"

#include <vulkan/vulkan.h>

vikaFrameBuffer::vikaFrameBuffer(vikaDevice *logDevice, vikaRenderPass *renderPass, vikaDepthBuffer *depthBuffer, VkExtent2D &imageSize) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_renderPass(renderPass),
	m_depthBuffer(depthBuffer),
	m_frameBuffer(VK_NULL_HANDLE)
{
	m_attachments.resize(2);

	m_bufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	m_bufferInfo.pNext = NULL;
	m_bufferInfo.flags = 0;
	//m_bufferInfo.renderPass = m_renderPass->m_renderpass;
	//m_bufferInfo.attachmentCount = m_attachments.size();
	//m_bufferInfo.pAttachments = m_attachments.data();
	m_bufferInfo.height = imageSize.height;
	m_bufferInfo.width = imageSize.width;
	m_bufferInfo.layers = 0;
}

vikaFrameBuffer::~vikaFrameBuffer()
{
	destroy();
}

bool vikaFrameBuffer::create()
{
    m_attachments[1] = m_depthBuffer->m_view;
	m_bufferInfo.renderPass = m_renderPass->m_renderpass;
	m_bufferInfo.attachmentCount = m_attachments.size();
	m_bufferInfo.pAttachments = m_attachments.data();

	m_res = vkCreateFramebuffer(m_logDevice->getDevice(), &m_bufferInfo, NULL, &m_frameBuffer);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaFrameBuffer::destroy()
{
	if (m_frameBuffer != VK_NULL_HANDLE)
	{
		vkDestroyFramebuffer(m_logDevice->getDevice(), m_frameBuffer, NULL);
		m_frameBuffer = VK_NULL_HANDLE;
	}
}

