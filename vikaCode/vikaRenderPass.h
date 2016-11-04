// vikaRenderPass : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKARENDERPASS_H_
#define _VIKARENDERPASS_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;

class vikaRenderPass
{
protected:
	VkResult m_res;
	vikaDevice *m_parent;

	std::vector<VkAttachmentDescription> m_attachments;

    VkAttachmentReference m_colorReference = {};
    VkAttachmentReference m_depthReference = {};

    VkSubpassDescription m_subpassDesc = {};

    VkRenderPassCreateInfo m_renderpassInfo = {};
	VkRenderPass m_renderpass;

public:
	vikaRenderPass(vikaDevice *parent);
	virtual ~vikaRenderPass();

	bool create();
	void destroy();
};

#endif 
