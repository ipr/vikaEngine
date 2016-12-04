// vikaIndexBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAINDEXBUFFER_H_
#define _VIKAINDEXBUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaPhysDevice;
class vikaCommandBuffer;
//class vikaSemaphore;
class vikaBuffer;

class vikaIndexBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;
	vikaCommandBuffer *m_commandBuffer;
	//vikaSemaphore *m_semaphore;
	vikaBuffer *m_buffer;

public: // simplify things..
	VkDescriptorBufferInfo m_descInfo = {};
	VkIndexType m_indexType;

public:
	vikaIndexBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, vikaCommandBuffer *commandBuffer);
	virtual ~vikaIndexBuffer();

	bool create(VkDeviceSize bufferSize, VkIndexType indexType = VK_INDEX_TYPE_UINT32);
	void destroy();

	// parameter expected: index data and size of it
	bool copyToMemory(uint32_t sizeIndices, void *dataIndices);

	void bindIndexBuffer(uint32_t cmdBufferIndex = 0);
};

#endif
