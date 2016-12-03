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

// split "device memory" allocation from buffer code:
// allocation can be used for various cases
class vikaDevMemory
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;

	// if mapping active
	bool m_isMapped;

public: // simplify things..
	VkDeviceMemory m_devMemory;
	VkMemoryAllocateInfo m_memInfo = {};

public:
	vikaDevMemory(vikaDevice *logDevice, vikaPhysDevice *physDevice);
	virtual ~vikaDevMemory();

	bool create(VkDeviceSize bufferSize);
	void destroy();

	uint8_t *mapMem(VkDeviceSize size, VkDeviceSize offset = 0);
	void unmapMem();
};

class vikaBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;
	vikaDevMemory *m_devMemory;

public: // simplify things..
    VkBufferCreateInfo m_bufferInfo = {};
	VkBuffer m_buffer;

	VkMemoryRequirements m_memReqs = {};

public:
	vikaBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice, VkBufferUsageFlags usage);
	virtual ~vikaBuffer();

	bool create(VkDeviceSize bufferSize, VkFlags reqMask);
	void destroy();

	bool copyToMemory(uint32_t size, void *data);
	bool bindMemory();
};

#endif
