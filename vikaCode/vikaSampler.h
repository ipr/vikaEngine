// vikaSampler : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASAMPLER_H_
#define _VIKASAMPLER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;

class vikaSampler
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;

public:
	VkSamplerCreateInfo m_samplerInfo = {};
	VkSampler m_sampler;

public:
	vikaSampler(vikaDevice *logDevice);
	virtual ~vikaSampler();

	bool create();
	void destroy();
};

#endif
