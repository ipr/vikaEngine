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
class vikaVertexBuffer;
class vikaPipeline;
class vikaDescriptorset;
class vikaSemaphore;
class vikaImageMemoryBarrier;

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
	vikaVertexBuffer *m_vertexBuffer;
	vikaPipeline *m_pipeline;
	vikaDescriptorset *m_descriptorSet;
	vikaSemaphore *m_semaphore;

	vikaImageMemoryBarrier *m_imageMemoryBarrier;

public: // simplify..
	uint32_t m_imageIndex;

	std::vector<VkAttachmentDescription> m_attachments;
	std::vector<VkAttachmentReference> m_attachRefs;

    VkSubpassDescription m_subpassDesc = {};

    VkRenderPassCreateInfo m_renderpassInfo = {};
	VkRenderPass m_renderpass;

	VkPresentInfoKHR m_presentInfo = {};

	VkExtent2D m_granularity;

public:
	vikaRenderPass(vikaDevice *device, vikaSurface *surface, vikaSwapChain *swapchain, vikaCommandBuffer *commandBuffer, vikaDepthBuffer *depthBuffer, vikaFrameBuffer *framebuffer, vikaVertexBuffer *vertexBuffer, vikaPipeline *pipeline, vikaDescriptorset *descriptorSet);
	virtual ~vikaRenderPass();

	bool create(VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT);
	void destroy();

	bool beginPass(VkSubpassContents subpass);
	void endPass();

	// must be within renderpass begin()/end() for this?
	void bindVertexBuffer();
	void bindPipeline();
	void bindDescriptorSets();

	bool presentImage();

	void getGranularity();
};

#endif 
