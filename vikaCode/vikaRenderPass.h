// vikaRenderPass : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKARENDERPASS_H_
#define _VIKARENDERPASS_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaSurface;
class vikaSwapChain;
class vikaCommandBuffer;
class vikaDepthBuffer;
class vikaFrameBuffer;
class vikaSemaphore;

class vikaRenderPass
{
protected:
	VkResult m_res;
	vikaDevice *m_device;
	vikaSurface *m_surface;
	vikaSwapChain *m_swapchain;
	vikaCommandBuffer *m_commandBuffer;
	vikaDepthBuffer *m_depthBuffer;
	vikaFrameBuffer *m_framebuffer;
	vikaSemaphore *m_semaphore;

public: // simplify..
    VkImageMemoryBarrier m_imageMemoryBarrier = {};

	std::vector<VkAttachmentDescription> m_attachments;

    VkAttachmentReference m_colorReference = {};
    VkAttachmentReference m_depthReference = {};

    VkSubpassDescription m_subpassDesc = {};

    VkRenderPassCreateInfo m_renderpassInfo = {};
	VkRenderPass m_renderpass;

public:
	vikaRenderPass(vikaDevice *device, vikaSurface *surface, vikaSwapChain *swapchain, vikaCommandBuffer *commandBuffer, vikaDepthBuffer *depthBuffer, vikaFrameBuffer *framebuffer);
	virtual ~vikaRenderPass();

	bool create(VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT);
	void destroy();

	bool acquireImage();

	void beginPass(VkSubpassContents subpass);
	void endPass();

	void createImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask, VkImage &image);
};

#endif 
