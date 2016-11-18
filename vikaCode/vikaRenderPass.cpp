// vikaRenderPass : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaRenderPass.h"
#include "vikaDevice.h"
#include "vikaDepthBuffer.h"

#include <vulkan/vulkan.h>

vikaRenderPass::vikaRenderPass(vikaDevice *device, vikaDepthBuffer *depthBuffer) :
	m_res(VK_SUCCESS),
	m_device(device),
	m_renderpass(VK_NULL_HANDLE),
	m_depthBuffer(depthBuffer)
{
	// attachments for render target and depth buffer
	m_attachments.resize(2);

    m_colorReference.attachment = 0;
	m_colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	m_depthReference.attachment = 1;
	m_depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    m_subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    m_subpassDesc.flags = 0;
    m_subpassDesc.inputAttachmentCount = 0;
    m_subpassDesc.pInputAttachments = NULL;
    m_subpassDesc.colorAttachmentCount = 1;
    m_subpassDesc.pColorAttachments = &m_colorReference;
    m_subpassDesc.pResolveAttachments = NULL;
    m_subpassDesc.pDepthStencilAttachment = &m_depthReference;
    m_subpassDesc.preserveAttachmentCount = 0;
    m_subpassDesc.pPreserveAttachments = NULL;

    m_renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    m_renderpassInfo.pNext = NULL;
    m_renderpassInfo.attachmentCount = 2;
    m_renderpassInfo.pAttachments = m_attachments.data();
    m_renderpassInfo.subpassCount = 1;
    m_renderpassInfo.pSubpasses = &m_subpassDesc;
    m_renderpassInfo.dependencyCount = 0;
    m_renderpassInfo.pDependencies = NULL;
}

vikaRenderPass::~vikaRenderPass()
{
	destroy();
}

bool vikaRenderPass::create(VkSampleCountFlagBits sampleCount)
{
    m_res = vkCreateRenderPass(m_device->getDevice(), &m_renderpassInfo, NULL, &m_renderpass);
    if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaRenderPass::destroy()
{
	if (m_renderpass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(m_device->getDevice(), m_renderpass, NULL);
		m_renderpass = VK_NULL_HANDLE;
	}
}
