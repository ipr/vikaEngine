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
class vikaVertexBuffer;
class vikaRenderPass;

class vikaPipeline
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	//vikaDescriptorset *m_descSet;
	vikaUniformBuffer *m_uniBuffer;
	//vikaVertexBuffer *m_vertexBuffer;

public: // simplify things..

	// TODO: move
    VkDescriptorSetLayoutBinding m_layoutBinding = {};
    VkDescriptorSetLayoutCreateInfo m_descriptorLayout = {};
	std::vector<VkDescriptorSetLayout> m_layouts;

    VkPipelineLayoutCreateInfo m_pipelineInfo = {};
	VkPipelineLayout m_pipelineLayout;

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
	vikaPipeline(vikaDevice *logDevice, vikaUniformBuffer *uniBuffer);
	virtual ~vikaPipeline();

	void destroy();

	bool createLayout(uint32_t descriptorSetCount = 1);

	// count of viewports and scissor should be equal
	bool createInputAssembly(vikaVertexBuffer *vertexBuffer, vikaRenderPass *renderpass, VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT, uint32_t viewportCount = 1, uint32_t scissorsCount = 1);
};

#endif
