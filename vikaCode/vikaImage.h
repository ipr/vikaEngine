// vikaImage : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAIMAGE_H_
#define _VIKAIMAGE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;

class vikaImage
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;

public: // simplify things..
	VkImage m_image;
    VkImageCreateInfo m_imageInfo = {};

	VkMemoryRequirements m_memReqs = {};

public:
	vikaImage(vikaDevice *logDevice);
	virtual ~vikaImage();

	bool create();
	void destroy();
};

#endif
