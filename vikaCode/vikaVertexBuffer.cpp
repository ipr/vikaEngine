// vikaVertexBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaVertexBuffer.h"
#include "vikaDevice.h"
#include "vikaRenderPass.h"
#include "vikaSemaphore.h"

#include <vulkan/vulkan.h>

vikaVertexBuffer::vikaVertexBuffer(vikaDevice *logDevice, vikaRenderPass *renderPass) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
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

