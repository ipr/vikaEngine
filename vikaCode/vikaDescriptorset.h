// vikaDescriptorset : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKADESCRIPTORSET_H_
#define _VIKADESCRIPTORSET_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;

class vikaDescriptorset
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;

	std::vector<VkDescriptorPoolSize> m_poolsize;
    VkDescriptorPoolCreateInfo m_descriptorPool = {};
	VkDescriptorPool m_pool;

	//VkDescriptorSetLayout m_setLayout;
	//VkDescriptorSet m_descriptorset;

    std::vector<VkDescriptorSetAllocateInfo> m_allocInfo;
    std::vector<VkWriteDescriptorSet> m_writeSet;

public:
	vikaDescriptorset(vikaDevice *logDevice);
	virtual ~vikaDescriptorset();

	bool create();
	void destroy();
};

#endif
