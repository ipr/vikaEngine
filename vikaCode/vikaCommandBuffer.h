// vikaCommandBuffer : manage pool and buffers for commands
//
// vulkan pool should handle most of the work, define only wrapper to help with API
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKACOMMANDBUFFER_H_
#define _VIKACOMMANDBUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaPhysDevice;

class vikaCommandBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;

public: // simplify things..
    VkCommandPoolCreateInfo m_cmdPoolInfo = {};
	VkCommandPool m_cmdPool;

    VkCommandBufferAllocateInfo m_cmdBufferInfo = {};
	std::vector<VkCommandBuffer> m_cmdBuffers;

public:
	vikaCommandBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice);
	virtual ~vikaCommandBuffer();

	bool create(uint32_t bufferCount = 1);
	void destroy();

	bool resetPool();
	bool resetBuffer(uint32_t bufferIndex = 0);

	bool executeEnd(uint32_t bufferIndex = 0);

	VkCommandBuffer& getCmd(uint32_t bufferIndex = 0) { return m_cmdBuffers[bufferIndex]; };
};

#endif
