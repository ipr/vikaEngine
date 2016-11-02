// vikaDepthBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKADEPTHBUFFER_H_
#define _VIKADEPTHBUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;

class vikaDepthBuffer
{
protected:
	vikaDevice *m_parent;
	VkResult m_res;

public:
	vikaDepthBuffer(vikaDevice *parent);
	virtual ~vikaDepthBuffer();

	bool create();
	void destroy();
};

#endif
