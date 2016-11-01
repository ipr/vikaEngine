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

class vikaCommandBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_parent;

	uint32_t m_queueIndex;
	//uint32_t m_bufferCount;

    VkCommandPoolCreateInfo m_cmdPoolInfo = {};
	VkCommandPool m_cmdPool;

    VkCommandBufferAllocateInfo m_cmdBufferInfo = {};
	std::vector<VkCommandBuffer> m_cmdBuffers;

public:
	vikaCommandBuffer(vikaDevice *parent, const uint32_t queueIndex);
	~vikaCommandBuffer();

	bool create(uint32_t bufferCount = 1);
	void destroy();

	bool resetPool();
	bool resetBuffer(uint32_t bufferIndex = 0);
};

#endif
