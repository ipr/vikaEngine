// vikaPipeline : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAPIPELINE_H_
#define _VIKAPIPELINE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;

class vikaPipeline
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;

    VkDescriptorSetLayoutBinding m_layoutBinding = {};
    VkDescriptorSetLayoutCreateInfo m_descriptorLayout = {};

public:
	vikaPipeline(vikaDevice *logDevice);
	virtual ~vikaPipeline();

	bool create();
	void destroy();
};

#endif
