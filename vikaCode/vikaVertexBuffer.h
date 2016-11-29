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
//class vikaSemaphore;

class vikaVertexBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;
	vikaCommandBuffer *m_commandBuffer;
	//vikaSemaphore *m_semaphore;

public: // simplify things..
    VkBufferCreateInfo m_bufferInfo = {};
	VkBuffer m_buffer;

	VkDeviceMemory m_devMemory;
	VkMemoryRequirements m_memReqs = {};
	VkMemoryAllocateInfo m_memInfo = {};

	VkDescriptorBufferInfo m_descInfo = {};

	std::vector<VkClearValue> m_clearValues;

	//VkVertexInputBindingDescription m_viBinding = {};
	std::vector<VkVertexInputBindingDescription> m_viBindings;
	std::vector<VkVertexInputAttributeDescription> m_viAttribs;

public:
	vikaVertexBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, vikaCommandBuffer *commandBuffer);
	virtual ~vikaVertexBuffer();

	bool create(VkDeviceSize bufferSize, uint32_t strideSize);
	void destroy();

	// parameter expected: vertex data and size of it
	bool copyToMemory(uint32_t sizeVertices, void *dataVertices);
};

#endif
