// vikaPipeline : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAPIPELINE_H_
#define _VIKAPIPELINE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaCommandBuffer;
class vikaDescriptorset;
class vikaUniformBuffer;
class vikaVertexBuffer;
class vikaRenderPass;
class vikaShaderModule;

class vikaPipeline
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	//vikaDescriptorset *m_descSet;
	//vikaUniformBuffer *m_uniBuffer;
	//vikaVertexBuffer *m_vertexBuffer;

public: // simplify things..

	// TODO: move
    VkDescriptorSetLayoutBinding m_layoutBinding = {};
    VkDescriptorSetLayoutCreateInfo m_descriptorLayout = {};
	std::vector<VkDescriptorSetLayout> m_layouts;

    VkPipelineLayoutCreateInfo m_pipelineInfo = {};
	VkPipelineLayout m_pipelineLayout;

	VkDynamicState m_dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
	VkPipelineDynamicStateCreateInfo m_dynamicState = {};

	VkPipelineVertexInputStateCreateInfo m_vertexInput = {};
	VkPipelineInputAssemblyStateCreateInfo m_pipelineInput = {};
	VkPipelineRasterizationStateCreateInfo m_pipelineRaster = {};

	std::vector<VkPipelineColorBlendAttachmentState> m_blendAttachments;
	VkPipelineColorBlendStateCreateInfo m_colorBlend = {};

	// count of viewports and scissor should be equal
	//uint32_t m_viewportCount;
	//uint32_t m_scissorsCount;

    VkPipelineViewportStateCreateInfo m_viewport = {};
	VkPipelineDepthStencilStateCreateInfo m_depthstencil = {};
	VkPipelineMultisampleStateCreateInfo m_multisample = {};

	VkGraphicsPipelineCreateInfo m_graphicsPipelineInfo = {};

	//VkPipelineCache ..
	VkPipeline m_pipeline;

public:
	vikaPipeline(vikaDevice *logDevice);
	virtual ~vikaPipeline();

	void destroy();

	bool createLayout(uint32_t descriptorSetCount = 1);

	void setVertexBuffer(vikaVertexBuffer *vertexBuffer);
	void setShaders(vikaShaderModule *shaders);

	// count of viewports and scissor should be equal
	bool createInputAssembly(vikaRenderPass *renderpass, VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT, uint32_t viewportCount = 1, uint32_t scissorsCount = 1);
};

#endif
