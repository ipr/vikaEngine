// vikaRenderPass : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaRenderPass.h"
#include "vikaDevice.h"
#include "vikaSurface.h"
#include "vikaSwapChain.h"
#include "vikaCommandBuffer.h"
#include "vikaDepthBuffer.h"
#include "vikaFrameBuffer.h"
#include "vikaVertexBuffer.h"
#include "vikaPipeline.h"
#include "vikaDescriptorset.h"
#include "vikaSemaphore.h"
#include "vikaBarrier.h"

#include <vulkan/vulkan.h>


vikaRenderPass::vikaRenderPass(vikaDevice *device, vikaSurface *surface, vikaSwapChain *swapchain, 
	vikaCommandBuffer *commandBuffer, vikaDepthBuffer *depthBuffer, vikaFrameBuffer *framebuffer, 
	vikaVertexBuffer *vertexBuffer, vikaPipeline *pipeline, vikaDescriptorset *descriptorSet) :
	m_res(VK_SUCCESS),
	m_device(device),
	m_surface(surface),
	m_swapchain(swapchain),
	m_commandBuffer(commandBuffer),
	m_depthBuffer(depthBuffer),
	m_framebuffer(framebuffer),
	m_vertexBuffer(vertexBuffer),
	m_pipeline(pipeline),
	m_descriptorSet(descriptorSet),
	m_semaphore(nullptr),
	m_imageMemoryBarrier(nullptr),
	m_imageIndex(0),
	m_renderpass(VK_NULL_HANDLE)
{
	// attachments for render target and depth buffer
	m_attachments.resize(2);
	//m_attachments[0].format = m_surface->m_format;
	//m_attachments[0].samples = sampleCount; // filled in later
	m_attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	m_attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	m_attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	m_attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	m_attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	m_attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	m_attachments[0].flags = 0;

	//m_attachments[1].format = m_depthBuffer->m_depthFormat;
	//m_attachments[1].samples = sampleCount; // filled in later
	m_attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	m_attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	m_attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	m_attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	m_attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	m_attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	m_attachments[1].flags = 0;

	m_attachRefs.resize(2);
	m_attachRefs[0].attachment = 0;
	m_attachRefs[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	m_attachRefs[1].attachment = 1;
	m_attachRefs[1].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	/*
    m_colorReference.attachment = 0;
	m_colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	m_depthReference.attachment = 1;
	m_depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	*/

	m_subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	m_subpassDesc.flags = 0;
	m_subpassDesc.inputAttachmentCount = 0;
	m_subpassDesc.pInputAttachments = NULL;
	m_subpassDesc.colorAttachmentCount = 1;
	m_subpassDesc.pColorAttachments = &m_attachRefs[0];
	m_subpassDesc.pResolveAttachments = NULL;
	m_subpassDesc.pDepthStencilAttachment = &m_attachRefs[1];
	m_subpassDesc.preserveAttachmentCount = 0;
	m_subpassDesc.pPreserveAttachments = NULL;

	m_renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	m_renderpassInfo.pNext = NULL;
	//m_renderpassInfo.attachmentCount = m_attachments.size();
	//m_renderpassInfo.pAttachments = m_attachments.data();
	m_renderpassInfo.subpassCount = 1;
	m_renderpassInfo.pSubpasses = &m_subpassDesc;
	m_renderpassInfo.dependencyCount = 0;
	m_renderpassInfo.pDependencies = NULL;

	m_presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	m_presentInfo.pNext = NULL;
	m_presentInfo.swapchainCount = 1;
	m_presentInfo.pSwapchains = NULL;
	m_presentInfo.pImageIndices = NULL;
	m_presentInfo.pWaitSemaphores = NULL;
	m_presentInfo.waitSemaphoreCount = 0;
	m_presentInfo.pResults = NULL;
}

vikaRenderPass::~vikaRenderPass()
{
	destroy();
}

bool vikaRenderPass::create(VkSampleCountFlagBits sampleCount)
{
	m_attachments[0].format = m_surface->m_format;
	m_attachments[0].samples = sampleCount;
	m_attachments[1].format = m_depthBuffer->m_depthFormat;
	m_attachments[1].samples = sampleCount;

	m_renderpassInfo.attachmentCount = m_attachments.size();
	m_renderpassInfo.pAttachments = m_attachments.data();

	// expecting that depth buffer has been set before this..
	m_semaphore = new vikaSemaphore(m_device->getDevice());
	if (m_semaphore->create() == false)
	{
		return false;
	}

    // Acquire the swapchain image index in order to set its layout
	//uint32_t imageIndex = 0;
    m_res = vkAcquireNextImageKHR(m_device->getDevice(), m_swapchain->m_swapchain, 
								UINT64_MAX, m_semaphore->getSemaphore(), VK_NULL_HANDLE,
                                &m_imageIndex);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_imageMemoryBarrier = new vikaBarrier(VK_IMAGE_LAYOUT_UNDEFINED, 
											VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
											VK_IMAGE_ASPECT_COLOR_BIT, 
											m_swapchain->m_swapchainImages[m_imageIndex]);
	m_imageMemoryBarrier->create(m_commandBuffer->getCmd(0));

    m_res = vkCreateRenderPass(m_device->getDevice(), &m_renderpassInfo, NULL, &m_renderpass);
    if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaRenderPass::destroy()
{
	if (m_imageMemoryBarrier != nullptr)
	{
		//m_imageMemoryBarrier->destroy();
		delete m_imageMemoryBarrier;
		m_imageMemoryBarrier = nullptr;
	}

	if (m_renderpass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(m_device->getDevice(), m_renderpass, NULL);
		m_renderpass = VK_NULL_HANDLE;
	}

	if (m_semaphore != nullptr)
	{
		m_semaphore->destroy();
		delete m_semaphore;
		m_semaphore = nullptr;
	}
}

bool vikaRenderPass::beginPass(VkSubpassContents subpass)
{
	//VkSubpassContents subpass = VK_SUBPASS_CONTENTS_INLINE;
	VkRenderPassBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	beginInfo.pNext = NULL;
	beginInfo.renderPass = m_renderpass;
	beginInfo.framebuffer = m_framebuffer->m_frameBuffers[m_imageIndex];
	beginInfo.renderArea.offset.x = 0;
	beginInfo.renderArea.offset.y = 0;
	beginInfo.renderArea.extent.height = m_framebuffer->m_bufferInfo.height;
	beginInfo.renderArea.extent.width = m_framebuffer->m_bufferInfo.width;
	beginInfo.clearValueCount = m_vertexBuffer->m_clearValues.size();
	beginInfo.pClearValues = m_vertexBuffer->m_clearValues.data();

	vkCmdBeginRenderPass(m_commandBuffer->getCmd(0), &beginInfo, subpass);
	return true;
}

void vikaRenderPass::endPass()
{
	vkCmdEndRenderPass(m_commandBuffer->getCmd(0));
}

// must be within renderpass begin()/end() for this?
void vikaRenderPass::bindVertexBuffer()
{
	// not sure what to do with this..
    const VkDeviceSize offsets[1] = {0};

	vkCmdBindVertexBuffers(m_commandBuffer->getCmd(0), // likely same as used in renderpass (if more than one)
							0,				// Start Binding 
							1,				// Binding Count 
							&m_vertexBuffer->m_buffer,		// pBuffers 
							offsets);		// pOffsets 
}

void vikaRenderPass::bindPipeline()
{
	//m_pipeline(pipeline),
	vkCmdBindPipeline(m_commandBuffer->getCmd(0), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->m_pipeline);
}

void vikaRenderPass::bindDescriptorSets()
{
	//m_descriptorSet(descriptorSet),
	vkCmdBindDescriptorSets(m_commandBuffer->getCmd(0), VK_PIPELINE_BIND_POINT_GRAPHICS,
							m_pipeline->m_pipelineLayout, 
							0, 
							m_descriptorSet->m_descriptorset.size(),
							m_descriptorSet->m_descriptorset.data(), 
							0, NULL);
}

bool vikaRenderPass::presentImage()
{
	m_presentInfo.pSwapchains = &m_swapchain->m_swapchain;
	m_presentInfo.pImageIndices = &m_imageIndex;

	m_res = vkQueuePresentKHR(m_device->getPresentQueue(), &m_presentInfo);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

