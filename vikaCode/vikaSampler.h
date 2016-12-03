// vikaSampler : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASAMPLER_H_
#define _VIKASAMPLER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaPhysDevice;
class vikaDevMemory;
class vikaImage;

class vikaSampler
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;
	vikaDevMemory *m_devMemory;
	vikaImage *m_image;

public:
	VkSamplerCreateInfo m_samplerInfo = {};
	VkSampler m_sampler;

public:
	vikaSampler(vikaDevice *logDevice, vikaPhysDevice *physDevice);
	virtual ~vikaSampler();

	bool create();
	void destroy();
};

#endif
