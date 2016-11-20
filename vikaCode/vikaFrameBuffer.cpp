// vikaFrameBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaFrameBuffer.h"
#include "vikaDevice.h"
#include "vikaRenderPass.h"

#include <vulkan/vulkan.h>

vikaFrameBuffer::vikaFrameBuffer(vikaDevice *logDevice, vikaRenderPass *renderPass) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_renderPass(renderPass),
	m_frameBuffer(VK_NULL_HANDLE)
{
	m_bufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	m_bufferInfo.pNext = NULL;

}

vikaFrameBuffer::~vikaFrameBuffer()
{
	destroy();
}

bool vikaFrameBuffer::create()
{
	m_bufferInfo.renderPass = m_renderPass->m_renderpass;

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

