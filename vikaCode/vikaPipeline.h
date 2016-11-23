// vikaPipeline : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAPIPELINE_H_
#define _VIKAPIPELINE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaDescriptorset;
class vikaUniformBuffer;

class vikaPipeline
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	//vikaDescriptorset *m_descSet;
	vikaUniformBuffer *m_uniBuffer;

public: // simplify things..

	// TODO: move
    VkDescriptorSetLayoutBinding m_layoutBinding = {};
    VkDescriptorSetLayoutCreateInfo m_descriptorLayout = {};
	std::vector<VkDescriptorSetLayout> m_layouts;

    VkPipelineLayoutCreateInfo m_pipelineInfo = {};
	VkPipelineLayout m_pipelineLayout;

	//VkPipelineCache ..

public:
	vikaPipeline(vikaDevice *logDevice, vikaUniformBuffer *uniBuffer);
	virtual ~vikaPipeline();

	bool create(uint32_t descriptorSetCount = 1);
	void destroy();
};

#endif
