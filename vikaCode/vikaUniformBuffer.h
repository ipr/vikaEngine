// vikaUniformBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAUNIFORMBUFFER_H_
#define _VIKAUNIFORMBUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaPhysDevice;

class vikaUniformBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;

public: // simplify things..
	VkDeviceSize m_bufferSize;
    VkBufferCreateInfo m_bufferInfo = {};
	VkBuffer m_buffer;

	VkDeviceMemory m_devMemory;
	VkMemoryRequirements m_memReqs = {};
	VkMemoryAllocateInfo m_memInfo = {};

	VkDescriptorBufferInfo m_descInfo = {};

public:
	vikaUniformBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice);
	virtual ~vikaUniformBuffer();

	bool create(const uint32_t bufferSize);
	void destroy();
};

#endif
