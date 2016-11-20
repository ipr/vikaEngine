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

class vikaFrameBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaRenderPass *m_renderPass;

	VkFramebufferCreateInfo m_bufferInfo = {};
	VkFramebuffer m_frameBuffer;

public:
	vikaFrameBuffer(vikaDevice *logDevice, vikaRenderPass *renderPass);
	virtual ~vikaFrameBuffer();

	bool create();
	void destroy();
};

#endif
