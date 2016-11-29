// vikaDescriptorset : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKADESCRIPTORSET_H_
#define _VIKADESCRIPTORSET_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaPipeline;
class vikaUniformBuffer;

class vikaDescriptorset
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPipeline *m_pipeline;
	vikaUniformBuffer *m_uniBuffer;

public: // simplify..
	std::vector<VkDescriptorPoolSize> m_poolsize;
    VkDescriptorPoolCreateInfo m_descriptorPool = {};
	VkDescriptorPool m_pool;

    std::vector<VkDescriptorSetAllocateInfo> m_allocInfo;
	std::vector<VkDescriptorSet> m_descriptorset;
    std::vector<VkWriteDescriptorSet> m_writeSet;

public:
	vikaDescriptorset(vikaDevice *logDevice, vikaPipeline *pipeline, vikaUniformBuffer *uniBuffer);
	virtual ~vikaDescriptorset();

	bool create(uint32_t descriptorSetCount = 1);
	void destroy();
};

#endif
