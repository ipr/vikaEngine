// vikaVertexBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAVERTEXBUFFER_H_
#define _VIKAVERTEXBUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaPhysDevice;
class vikaCommandBuffer;
class vikaRenderPass;
class vikaSemaphore;

class vikaVertexBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;
	vikaCommandBuffer *m_commandBuffer;
	vikaRenderPass *m_renderPass;
	vikaSemaphore *m_semaphore;

public: // simplify things..
	VkDeviceSize m_bufferSize;
    VkBufferCreateInfo m_bufferInfo = {};
	VkBuffer m_buffer;

	VkDeviceMemory m_devMemory;
	VkMemoryRequirements m_memReqs = {};
	VkMemoryAllocateInfo m_memInfo = {};

	VkDescriptorBufferInfo m_descInfo = {};

	VkVertexInputBindingDescription m_viBinding = {};
	std::vector<VkVertexInputAttributeDescription> m_viAttribs;

public:
	vikaVertexBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, vikaCommandBuffer *commandBuffer, vikaRenderPass *renderPass, VkDeviceSize bufferSize);
	virtual ~vikaVertexBuffer();

	bool create();
	void destroy();

	// parameter expected: vertex data and size of it
	bool copyToMemory(uint32_t sizeVertices, void *dataVertices);

	// TODO: rest of stuff
	//bool vertexBinding();
};

#endif
