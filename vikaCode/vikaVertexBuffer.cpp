// vikaVertexBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaVertexBuffer.h"
#include "vikaDevice.h"
#include "vikaCommandBuffer.h"
#include "vikaRenderPass.h"
#include "vikaSemaphore.h"

#include <vulkan/vulkan.h>

vikaVertexBuffer::vikaVertexBuffer(vikaDevice *logDevice, vikaCommandBuffer *commandBuffer, vikaRenderPass *renderPass) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_commandBuffer(commandBuffer),
	m_renderPass(renderPass),
	m_semaphore(nullptr)
{
}

vikaVertexBuffer::~vikaVertexBuffer()
{
	destroy();
}

bool vikaVertexBuffer::create()
{
	m_semaphore = new vikaSemaphore(m_logDevice->getDevice());
	if (m_semaphore->create() == false)
	{
		return false;
	}

	//VkClearValue..

	// TODO:
	//m_renderPass->beginPass(VK_SUBPASS_CONTENTS_INLINE);

	/*
    vkCmdBindVertexBuffers(info.cmd, 0,             // Start Binding 
                           1,                       // Binding Count 
                           &info.vertex_buffer.buf, // pBuffers 
                           offsets);                // pOffsets 
						   */

	//m_renderPass->endPass();
	return true;
}

void vikaVertexBuffer::destroy()
{
	if (m_semaphore != nullptr)
	{
		m_semaphore->destroy();
		delete m_semaphore;
		m_semaphore = nullptr;
	}
}

