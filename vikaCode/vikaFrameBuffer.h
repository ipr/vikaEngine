// vikaFrameBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAFRAMEBUFFER_H_
#define _VIKAFRAMEBUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaRenderPass;
class vikaDepthBuffer;
class vikaSwapChain;
class vikaCommandBuffer;

class vikaFrameBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaDepthBuffer *m_depthBuffer;
	vikaSwapChain *m_swapchain;
	vikaCommandBuffer *m_commandBuffer;

public: // simplify..
	std::vector<VkImageView> m_attachments;

	VkFramebufferCreateInfo m_bufferInfo = {};
	std::vector<VkFramebuffer> m_frameBuffers;

public:
	vikaFrameBuffer(vikaDevice *logDevice, vikaDepthBuffer *depthBuffer, vikaSwapChain *swapchain, vikaCommandBuffer *commandBuffer);
	virtual ~vikaFrameBuffer();

	bool create(vikaRenderPass *renderPass, VkExtent2D &imageSize);
	void destroy();

	// count of viewports and scissor should be equal
	void setViewports(uint32_t viewportCount = 1);
	void setScissors(uint32_t scissorsCount = 1);
};

#endif
