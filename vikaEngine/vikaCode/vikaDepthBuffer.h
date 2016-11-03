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

class vikaDepthBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;

	VkFormatProperties m_formatProp = {};

	VkImage m_image;
    VkImageCreateInfo m_imageInfo = {};

	VkImageView m_view;
	VkImageViewCreateInfo m_viewInfo = {};

	VkDeviceMemory m_devMemory;
	VkMemoryRequirements m_memReqs = {};
	VkMemoryAllocateInfo m_memInfo = {};

public:
	vikaDepthBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice);
	virtual ~vikaDepthBuffer();

	bool create();
	void destroy();

	bool memtypeBitsToIndex(VkFlags reqMask = 0);
};

#endif