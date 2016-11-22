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

class vikaFrameBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaDepthBuffer *m_depthBuffer;
	vikaSwapChain *m_swapchain;

public: // simplify..
	std::vector<VkImageView> m_attachments;

	VkFramebufferCreateInfo m_bufferInfo = {};
	std::vector<VkFramebuffer> m_frameBuffers;

public:
	vikaFrameBuffer(vikaDevice *logDevice, vikaDepthBuffer *depthBuffer, vikaSwapChain *swapchain, VkExtent2D &imageSize);
	virtual ~vikaFrameBuffer();

	bool create(vikaRenderPass *renderPass);
	void destroy();
};

#endif
