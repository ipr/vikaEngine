// vikaRenderPass : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKARENDERPASS_H_
#define _VIKARENDERPASS_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaDepthBuffer;

class vikaRenderPass
{
protected:
	VkResult m_res;
	vikaDevice *m_device;
	vikaDepthBuffer *m_depthBuffer;

	std::vector<VkAttachmentDescription> m_attachments;

    VkAttachmentReference m_colorReference = {};
    VkAttachmentReference m_depthReference = {};

    VkSubpassDescription m_subpassDesc = {};

    VkRenderPassCreateInfo m_renderpassInfo = {};
	VkRenderPass m_renderpass;

public:
	vikaRenderPass(vikaDevice *device, vikaDepthBuffer *depthBuffer);
	virtual ~vikaRenderPass();

	bool create(VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT);
	void destroy();
};

#endif 
