// vikaSubpass : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASUBPASS_H_
#define _VIKASUBPASS_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaPhysDevice;

class vikaSubpass
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;

public:
	// dependency between source and destination subpasses
    VkSubpassDependency m_dependency = {};
    std::vector<VkSubpassDescription> m_subpasses;

public:
	vikaSubpass(vikaDevice *logDevice, vikaPhysDevice *physDevice);
	virtual ~vikaSubpass();

	bool create();
	void destroy();
};

#endif
