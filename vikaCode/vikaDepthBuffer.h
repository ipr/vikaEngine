// vikaDepthBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKADEPTHBUFFER_H_
#define _VIKADEPTHBUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaPhysDevice;
class vikaDevMemory;
class vikaImage;

class vikaDepthBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;
	vikaDevMemory *m_devMemory;
	vikaImage *m_image;

public: // simplify things..
	VkFormatProperties m_formatProp = {};
	VkFormat m_depthFormat;

	VkImageView m_view;
	VkImageViewCreateInfo m_viewInfo = {};
	VkMemoryRequirements m_memReqs = {};

public:
	vikaDepthBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice);
	virtual ~vikaDepthBuffer();

	bool create(VkExtent2D &imageSize, VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT, VkFormat depthFormat = VK_FORMAT_D16_UNORM);
	void destroy();
};

#endif
