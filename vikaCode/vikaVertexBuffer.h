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
class vikaBuffer;

class vikaVertexBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;
	vikaCommandBuffer *m_commandBuffer;
	//vikaSemaphore *m_semaphore;
	vikaBuffer *m_buffer;

	//std::vector<vikaBuffer*> m_buffers;

	// TODO: support array of buffers for better batching
	std::vector<VkBuffer> m_buffers; // for binding
	std::vector<VkDeviceSize> m_offsets; // for binding

public: // simplify things..
	std::vector<VkClearValue> m_clearValues;

	std::vector<VkVertexInputBindingDescription> m_viBindings;
	std::vector<VkVertexInputAttributeDescription> m_viAttribs;

	std::vector<VkDescriptorBufferInfo> m_descInfo;

public:
	vikaVertexBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, vikaCommandBuffer *commandBuffer);
	virtual ~vikaVertexBuffer();

	bool create(VkDeviceSize bufferSize, uint32_t strideSize);
	void destroy();

	// parameter expected: vertex data and size of it
	bool copyToMemory(uint32_t sizeVertices, void *dataVertices);

	void bindVertexBuffer(uint32_t cmdBufferIndex = 0);
};

#endif
