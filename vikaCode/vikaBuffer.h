// vikaBuffer : base buffer for various cases
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKABUFFER_H_
#define _VIKABUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaPhysDevice;

class vikaBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;

public: // simplify things..
    VkBufferCreateInfo m_bufferInfo = {};
	VkBuffer m_buffer;

	VkDeviceMemory m_devMemory;
	VkMemoryRequirements m_memReqs = {};
	VkMemoryAllocateInfo m_memInfo = {};

public:
	vikaBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, VkBufferUsageFlags usage);
	virtual ~vikaBuffer();

	bool create(VkDeviceSize bufferSize, VkFlags reqMask);
	void destroy();

	bool copyToMemory(uint32_t size, void *data);
};

#endif
