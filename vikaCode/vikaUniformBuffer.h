// vikaUniformBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAUNIFORMBUFFER_H_
#define _VIKAUNIFORMBUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;

class vikaUniformBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;

public:
	vikaUniformBuffer(vikaDevice *logDevice);
	virtual ~vikaUniformBuffer();

	bool create();
	void destroy();
};

#endif