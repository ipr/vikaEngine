// vikaVertexBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAVERTEXBUFFER_H_
#define _VIKAVERTEXBUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;

class vikaVertexBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;

public:
	vikaVertexBuffer(vikaDevice *logDevice);
	virtual ~vikaVertexBuffer();

	bool create();
	void destroy();
};

#endif
