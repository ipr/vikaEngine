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

class vikaFrameBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaRenderPass *m_renderPass;
	vikaDepthBuffer *m_depthBuffer;

	std::vector<VkImageView> m_attachments;

	VkFramebufferCreateInfo m_bufferInfo = {};
	VkFramebuffer m_frameBuffer;

public:
	vikaFrameBuffer(vikaDevice *logDevice, vikaRenderPass *renderPass, vikaDepthBuffer *depthBuffer, VkExtent2D &imageSize);
	virtual ~vikaFrameBuffer();

	bool create();
	void destroy();
};

#endif
