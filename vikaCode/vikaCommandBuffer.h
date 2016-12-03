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

#include "vikaFence.h"

class vikaDevice;

class vikaCommandBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;

public: // simplify things..
    VkCommandPoolCreateInfo m_cmdPoolInfo = {};
	VkCommandPool m_cmdPool;

    VkCommandBufferAllocateInfo m_cmdBufferInfo = {};
	std::vector<VkCommandBuffer> m_cmdBuffers;

	VkCommandBufferBeginInfo m_beginInfo = {};

	std::vector<VkViewport> m_viewports;
	std::vector<VkRect2D> m_scissors;

public:
	vikaCommandBuffer(vikaDevice *logDevice);
	virtual ~vikaCommandBuffer();

	bool create(uint32_t graphicsQueueIndex, uint32_t bufferCount = 1);
	void destroy();

	bool resetPool();
	bool resetBuffer(uint32_t bufferIndex = 0);

	void dispatch(uint32_t x, uint32_t y, uint32_t z, uint32_t bufferIndex = 0);
	//void dispatchIndirect(uint32_t x, uint32_t y, uint32_t z, uint32_t bufferIndex = 0);

	bool executeBegin(uint32_t bufferIndex = 0);
	bool executeEnd(uint32_t bufferIndex = 0);
	bool executeQueue(vikaFence &fence);

	void commandDraw(uint32_t verticesCount, uint32_t bufferIndex = 0);
	void commandDrawIndexed(uint32_t indexCount, uint32_t bufferIndex = 0);

	void commandLineWidth(float lineWidth, uint32_t bufferIndex = 0);
	void commandDepthBias(float cFactor, float clamp, float slope, uint32_t bufferIndex = 0);

	// count of viewports and scissor should be equal
	void setViewports(uint32_t width, uint32_t height, uint32_t viewportCount = 1, uint32_t bufferIndex = 0);
	void setScissors(uint32_t width, uint32_t height, uint32_t scissorsCount = 1, uint32_t bufferIndex = 0);
	void setBlendFactors(float constants[4], uint32_t bufferIndex = 0);
	void setDepthBounds(float min, float max, uint32_t bufferIndex = 0);
	void setStencilCompareMask(uint32_t bufferIndex = 0);
	void setStencilWriteMask(uint32_t bufferIndex = 0);
	void setStencilReference(uint32_t bufferIndex = 0);

	VkCommandBuffer& getCmd(uint32_t bufferIndex = 0) { return m_cmdBuffers[bufferIndex]; };
};

#endif
