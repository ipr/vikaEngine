// vikaVertexBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaVertexBuffer.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"
#include "vikaCommandBuffer.h"
#include "vikaRenderPass.h"
#include "vikaSemaphore.h"

#include <vulkan/vulkan.h>

vikaVertexBuffer::vikaVertexBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, vikaCommandBuffer *commandBuffer, vikaRenderPass *renderPass) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_commandBuffer(commandBuffer),
	m_renderPass(renderPass),
	m_semaphore(nullptr),
	m_buffer(VK_NULL_HANDLE),
	m_devMemory(VK_NULL_HANDLE)
{
	m_bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	m_bufferInfo.pNext = NULL;
	m_bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	m_bufferInfo.size = 0; 
	m_bufferInfo.queueFamilyIndexCount = 0;
	m_bufferInfo.pQueueFamilyIndices = NULL;
	m_bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	m_bufferInfo.flags = 0;

	m_memInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	m_memInfo.pNext = NULL;
	m_memInfo.memoryTypeIndex = 0;
}

vikaVertexBuffer::~vikaVertexBuffer()
{
	destroy();
}

bool vikaVertexBuffer::create(VkDeviceSize bufferSize)
{
	m_bufferInfo.size = bufferSize; 

	m_semaphore = new vikaSemaphore(m_logDevice->getDevice());
	if (m_semaphore->create() == false)
	{
		return false;
	}

    m_res = vkCreateBuffer(m_logDevice->getDevice(), &m_bufferInfo, NULL, &m_buffer);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_descInfo.buffer = m_buffer;
	m_descInfo.offset = 0;
	m_descInfo.range = m_bufferInfo.size;

	vkGetBufferMemoryRequirements(m_logDevice->getDevice(), m_buffer, &m_memReqs);

	// use m_memReqs.memoryTypeBits to get m_memInfo.memoryTypeIndex
	m_memInfo.allocationSize = m_memReqs.size;
	if (m_physDevice->memtypeBitsToIndex(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
										m_memReqs.memoryTypeBits, m_memInfo.memoryTypeIndex) == false)
	{
		return false;
	}

	m_res = vkAllocateMemory(m_logDevice->getDevice(), &m_memInfo, NULL, &m_devMemory);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return true;

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
	//return true;
}

void vikaVertexBuffer::destroy()
{
	if (m_devMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(m_logDevice->getDevice(), m_devMemory, NULL);
		m_devMemory = VK_NULL_HANDLE;
	}

	if (m_buffer != VK_NULL_HANDLE)
	{
	    vkDestroyBuffer(m_logDevice->getDevice(), m_buffer, NULL);
		m_buffer = VK_NULL_HANDLE;
	}

	if (m_semaphore != nullptr)
	{
		m_semaphore->destroy();
		delete m_semaphore;
		m_semaphore = nullptr;
	}
}

// parameter expected: vertex data and size of it
bool vikaVertexBuffer::copyToMemory(uint32_t sizeVertices, void *dataVertices)
{
	uint8_t *pMapping = nullptr; // pointer to mapping in user space (actually in device memory)
	m_res = vkMapMemory(m_logDevice->getDevice(), m_devMemory, 0, m_memReqs.size, 0, (void**)&pMapping);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	// copy to device via our mapping
	memcpy(pMapping, dataVertices, sizeVertices);
	vkUnmapMemory(m_logDevice->getDevice(), m_devMemory);

	m_res = vkBindBufferMemory(m_logDevice->getDevice(), m_buffer, m_devMemory, 0);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

bool vikaVertexBuffer::beginRender()
{
	m_clearValues.resize(2);
    m_clearValues[0].color.float32[0] = 0.2f;
    m_clearValues[0].color.float32[1] = 0.2f;
    m_clearValues[0].color.float32[2] = 0.2f;
    m_clearValues[0].color.float32[3] = 0.2f;
    m_clearValues[1].depthStencil.depth = 1.0f;
    m_clearValues[1].depthStencil.stencil = 0;

	if (m_renderPass->acquireImage() == false)
	{
		return false;
	}
	/*
	if (m_renderPass->beginPass() == false)
	{
		return false;
	}
	*/
	//m_renderPass->createImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, m_swapchain->m_swapchainImages[imageIndex]);
	//m_renderPass->beginPass();
	return true;
}
