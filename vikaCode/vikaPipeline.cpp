// vikaPipeline : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaPipeline.h"
#include "vikaDevice.h"
#include "vikaCommandBuffer.h"
#include "vikaDescriptorset.h"
#include "vikaUniformBuffer.h"
#include "vikaVertexBuffer.h"
#include "vikaRenderPass.h"
#include "vikaShaderModule.h"

#include <vulkan/vulkan.h>

vikaPipeline::vikaPipeline(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_pipelineLayout(VK_NULL_HANDLE),
	m_pipeline(VK_NULL_HANDLE)
{
	// TODO: when using textures, define sampler here
	m_layoutBinding.binding = 0;
	m_layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	m_layoutBinding.descriptorCount = 1;
	m_layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	m_layoutBinding.pImmutableSamplers = NULL;

	m_descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	m_descriptorLayout.pNext = NULL;
	m_descriptorLayout.bindingCount = 1;
	m_descriptorLayout.pBindings = &m_layoutBinding;

	m_pipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	m_pipelineInfo.pNext = NULL;
	m_pipelineInfo.pushConstantRangeCount = 0;
	m_pipelineInfo.pPushConstantRanges = NULL;
	//m_pipelineInfo.setLayoutCount = descriptorSetCount; // fill in later
	//m_pipelineInfo.pSetLayouts = m_layouts.data();

	m_pipelineRaster.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	m_pipelineRaster.pNext = NULL;
	m_pipelineRaster.flags = 0;
	m_pipelineRaster.polygonMode = VK_POLYGON_MODE_FILL;
	m_pipelineRaster.cullMode = VK_CULL_MODE_BACK_BIT;
	m_pipelineRaster.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	m_pipelineRaster.depthClampEnable = VK_TRUE;
	m_pipelineRaster.rasterizerDiscardEnable = VK_FALSE;
	m_pipelineRaster.depthBiasEnable = VK_FALSE;
	m_pipelineRaster.depthBiasConstantFactor = 0;
	m_pipelineRaster.depthBiasClamp = 0;
	m_pipelineRaster.depthBiasSlopeFactor = 0;
	m_pipelineRaster.lineWidth = 1.0f;

	m_pipelineInput.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	m_pipelineInput.pNext = NULL;
	m_pipelineInput.flags = 0;
	m_pipelineInput.primitiveRestartEnable = VK_FALSE;
	m_pipelineInput.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	m_depthstencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	m_depthstencil.pNext = NULL;
	m_depthstencil.flags = 0;
	m_depthstencil.depthTestEnable = VK_TRUE;
	m_depthstencil.depthWriteEnable = VK_TRUE;
	m_depthstencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	m_depthstencil.depthBoundsTestEnable = VK_FALSE;
	m_depthstencil.minDepthBounds = 0;
	m_depthstencil.maxDepthBounds = 0;
	m_depthstencil.stencilTestEnable = VK_FALSE;
	m_depthstencil.back.failOp = VK_STENCIL_OP_KEEP;
	m_depthstencil.back.passOp = VK_STENCIL_OP_KEEP;
	m_depthstencil.back.compareOp = VK_COMPARE_OP_ALWAYS;
	m_depthstencil.back.compareMask = 0;
	m_depthstencil.back.reference = 0;
	m_depthstencil.back.depthFailOp = VK_STENCIL_OP_KEEP;
	m_depthstencil.back.writeMask = 0;
	m_depthstencil.front = m_depthstencil.back;

	memset(m_dynamicStateEnables, 0, sizeof(VkDynamicState) * VK_DYNAMIC_STATE_RANGE_SIZE);
    m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    m_dynamicState.pNext = NULL;
    m_dynamicState.pDynamicStates = m_dynamicStateEnables;
    m_dynamicState.dynamicStateCount = 0;

	//VkPipelineVertexInputStateCreateInfo vi;
	m_vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_vertexInput.pNext = NULL;
	m_vertexInput.flags = 0;
	m_vertexInput.vertexBindingDescriptionCount = 0;
	m_vertexInput.pVertexBindingDescriptions = NULL;
	m_vertexInput.vertexAttributeDescriptionCount = 0;
	m_vertexInput.pVertexAttributeDescriptions = NULL;

	//VkGraphicsPipelineCreateInfo pipeline;
	m_graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	m_graphicsPipelineInfo.pNext = NULL;
	m_graphicsPipelineInfo.pStages = NULL;
	m_graphicsPipelineInfo.stageCount = 0;

}

vikaPipeline::~vikaPipeline()
{
	destroy();
}

void vikaPipeline::destroy()
{
	if (m_pipeline != VK_NULL_HANDLE)
	{
		vkDestroyPipeline(m_logDevice->getDevice(), m_pipeline, NULL);
		m_pipeline = VK_NULL_HANDLE;
	}

	if (m_pipelineLayout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(m_logDevice->getDevice(), m_pipelineLayout, NULL);
		m_pipelineLayout = VK_NULL_HANDLE;
	}

	for (size_t i = 0; i < m_layouts.size(); i++)
	{
		vkDestroyDescriptorSetLayout(m_logDevice->getDevice(), m_layouts[i], NULL);
	}
	m_layouts.clear();
}

// count of descriptorsets needs to be same in multiple places
bool vikaPipeline::createLayout(uint32_t descriptorSetCount)
{
	// TODO: move
	m_layouts.resize(descriptorSetCount);
	m_res = vkCreateDescriptorSetLayout(m_logDevice->getDevice(), &m_descriptorLayout, NULL, m_layouts.data());
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

    m_pipelineInfo.setLayoutCount = descriptorSetCount;
    m_pipelineInfo.pSetLayouts = m_layouts.data();

	m_res = vkCreatePipelineLayout(m_logDevice->getDevice(), &m_pipelineInfo, NULL, &m_pipelineLayout);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

void vikaPipeline::setVertexBuffer(vikaVertexBuffer *vertexBuffer)
{
	m_vertexInput.vertexBindingDescriptionCount = vertexBuffer->m_viBindings.size();
	m_vertexInput.pVertexBindingDescriptions = vertexBuffer->m_viBindings.data();
	m_vertexInput.vertexAttributeDescriptionCount = vertexBuffer->m_viAttribs.size();
	m_vertexInput.pVertexAttributeDescriptions = vertexBuffer->m_viAttribs.data();
}

void vikaPipeline::setShaders(vikaShaderModule *shaders)
{
	//if (shaders->m_shaderStages.size() > 0)
	m_graphicsPipelineInfo.pStages = shaders->m_shaderStages.data();
	m_graphicsPipelineInfo.stageCount = shaders->m_shaderStages.size();
}

bool vikaPipeline::createInputAssembly(vikaRenderPass *renderpass, VkSampleCountFlagBits sampleCount, uint32_t viewportCount, uint32_t scissorsCount)
{
	//VkPipelineColorBlendAttachmentState att_state[1];
	m_blendAttachments.resize(1);
	m_blendAttachments[0].colorWriteMask = 0xf;
	m_blendAttachments[0].blendEnable = VK_FALSE;
	m_blendAttachments[0].alphaBlendOp = VK_BLEND_OP_ADD;
	m_blendAttachments[0].colorBlendOp = VK_BLEND_OP_ADD;
	m_blendAttachments[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	m_blendAttachments[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	m_blendAttachments[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	m_blendAttachments[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

	//VkPipelineColorBlendStateCreateInfo cb;
	m_colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	m_colorBlend.pNext = NULL;
	m_colorBlend.flags = 0;
	m_colorBlend.attachmentCount = m_blendAttachments.size();
	m_colorBlend.pAttachments = m_blendAttachments.data();
	m_colorBlend.logicOpEnable = VK_FALSE;
	m_colorBlend.logicOp = VK_LOGIC_OP_NO_OP;
	m_colorBlend.blendConstants[0] = 1.0f;
	m_colorBlend.blendConstants[1] = 1.0f;
	m_colorBlend.blendConstants[2] = 1.0f;
	m_colorBlend.blendConstants[3] = 1.0f;

	//VkPipelineViewportStateCreateInfo vp = {};
	m_viewport.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	m_viewport.pNext = NULL;
	m_viewport.flags = 0;
	m_viewport.viewportCount = viewportCount;
	m_dynamicStateEnables[m_dynamicState.dynamicStateCount] = VK_DYNAMIC_STATE_VIEWPORT;
	m_dynamicState.dynamicStateCount++;
	m_viewport.scissorCount = scissorsCount;
	m_dynamicStateEnables[m_dynamicState.dynamicStateCount] = VK_DYNAMIC_STATE_SCISSOR;
	m_dynamicState.dynamicStateCount++;
	m_viewport.pScissors = NULL;
	m_viewport.pViewports = NULL;

	//VkPipelineMultisampleStateCreateInfo ms;
	m_multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	m_multisample.pNext = NULL;
	m_multisample.flags = 0;
	m_multisample.pSampleMask = NULL;
	m_multisample.rasterizationSamples = sampleCount;
	m_multisample.sampleShadingEnable = VK_FALSE;
	m_multisample.alphaToCoverageEnable = VK_FALSE;
	m_multisample.alphaToOneEnable = VK_FALSE;
	m_multisample.minSampleShading = 0.0;

	//VkGraphicsPipelineCreateInfo pipeline;
	m_graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	m_graphicsPipelineInfo.pNext = NULL;
	m_graphicsPipelineInfo.layout = m_pipelineLayout;
	m_graphicsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	m_graphicsPipelineInfo.basePipelineIndex = 0;
	m_graphicsPipelineInfo.flags = 0;
	m_graphicsPipelineInfo.pVertexInputState = &m_vertexInput;
	m_graphicsPipelineInfo.pInputAssemblyState = &m_pipelineInput;
	m_graphicsPipelineInfo.pRasterizationState = &m_pipelineRaster;
	m_graphicsPipelineInfo.pColorBlendState = &m_colorBlend;
	m_graphicsPipelineInfo.pTessellationState = NULL;
	m_graphicsPipelineInfo.pMultisampleState = &m_multisample;
	m_graphicsPipelineInfo.pDynamicState = &m_dynamicState;
	m_graphicsPipelineInfo.pViewportState = &m_viewport;
	m_graphicsPipelineInfo.pDepthStencilState = &m_depthstencil;
	//m_graphicsPipelineInfo.pStages = info.shaderStages; // set elsewhere
	//m_graphicsPipelineInfo.stageCount = 2;
	m_graphicsPipelineInfo.renderPass = renderpass->m_renderpass;
	m_graphicsPipelineInfo.subpass = 0;

    m_res = vkCreateGraphicsPipelines(m_logDevice->getDevice(), VK_NULL_HANDLE, 1, &m_graphicsPipelineInfo, NULL, &m_pipeline);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

