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

class vikaCommandBuffer
{
protected:
	VkResult m_res;

	uint32_t m_queueIndex;
	uint32_t m_bufferCount;

    VkCommandPoolCreateInfo m_cmdPoolInfo = {};
	VkCommandPool m_cmdPool;

    VkCommandBufferAllocateInfo m_cmdBufferInfo = {};
	std::vector<VkCommandBuffer> m_cmdBuffers;

public:
	vikaCommandBuffer(const uint32_t queueIndex, uint32_t bufferCount = 1);
	~vikaCommandBuffer();

	bool create(VkDevice &device);
	void destroy(VkDevice &device);

	bool resetPool(VkDevice &device);
	bool resetBuffer(VkDevice &device, uint32_t bufferIndex = 0);
};

#endif
