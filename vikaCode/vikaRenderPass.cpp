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
#include "vikaSemaphore.h"

#include <vulkan/vulkan.h>

vikaRenderPass::vikaRenderPass(vikaDevice *device, vikaSurface *surface, vikaSwapChain *swapchain, vikaCommandBuffer *commandBuffer, vikaDepthBuffer *depthBuffer) :
	m_res(VK_SUCCESS),
	m_device(device),
	m_surface(surface),
	m_swapchain(swapchain),
	m_commandBuffer(commandBuffer),
	m_depthBuffer(depthBuffer),
	m_semaphore(nullptr),
	m_renderpass(VK_NULL_HANDLE)
{
    m_imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    m_imageMemoryBarrier.pNext = NULL;
    m_imageMemoryBarrier.srcAccessMask = 0;
    m_imageMemoryBarrier.dstAccessMask = 0;
    //m_imageMemoryBarrier.oldLayout = oldLayout;
    //m_imageMemoryBarrier.newLayout = newLayout;
    m_imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    m_imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    //m_imageMemoryBarrier.image = image;
    //m_imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
    m_imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
    m_imageMemoryBarrier.subresourceRange.levelCount = 1;
    m_imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
    m_imageMemoryBarrier.subresourceRange.layerCount = 1;

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
    m_renderpassInfo.attachmentCount = m_attachments.size();
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
	m_attachments[0].format = m_surface->m_format;
	m_attachments[0].samples = sampleCount;
	m_attachments[1].format = m_depthBuffer->m_depthFormat;
	m_attachments[1].samples = sampleCount;

	// expecting that depth buffer has been set before this..
	m_semaphore = new vikaSemaphore(m_device->getDevice());
	if (m_semaphore->create() == false)
	{
		return false;
	}

    // Acquire the swapchain image index in order to set its layout
	uint32_t imageIndex = 0;
    m_res = vkAcquireNextImageKHR(m_device->getDevice(), m_swapchain->m_swapchain, UINT64_MAX, m_semaphore->getSemaphore(), VK_NULL_HANDLE,
                                &imageIndex);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	createImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, m_swapchain->m_swapchainImages[imageIndex]);

	// should execute "end" here?
	//m_commandBuffer->executeEnd();

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

	if (m_semaphore != nullptr)
	{
		m_semaphore->destroy();
		delete m_semaphore;
		m_semaphore = nullptr;
	}
}

void vikaRenderPass::beginPass(vikaFrameBuffer *framebuffer, VkSubpassContents subpass)
{
	/*
	//VkSubpassContents subpass = VK_SUBPASS_CONTENTS_INLINE;
	VkRenderPassBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	beginInfo.pNext = NULL;
	beginInfo.renderPass = m_renderpass;
	beginInfo.framebuffer = framebuffer->m_frameBuffer;
	beginInfo.renderArea = ;
	beginInfo.clearValueCount = ;
	beginInfo.pClearValues = NULL;

	vkCmdBeginRenderPass(m_commandBuffer->getCmd(0), &beginInfo, subpass);
	*/
}

void vikaRenderPass::endPass()
{
	/*
	vkCmdEndRenderPass(m_commandBuffer->getCmd(0));
	*/
}

void vikaRenderPass::createImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask, VkImage &image)
{
    m_imageMemoryBarrier.oldLayout = oldLayout;
    m_imageMemoryBarrier.newLayout = newLayout;
    m_imageMemoryBarrier.image = image;
    m_imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;

	switch (oldLayout)
	{
	case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        m_imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		break;
	case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        m_imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		break;
	case VK_IMAGE_LAYOUT_PREINITIALIZED:
        m_imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
		break;
	default:
		m_imageMemoryBarrier.srcAccessMask = 0;
		break;
	}

	switch (newLayout)
	{
	case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        m_imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		break;
	case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        m_imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		break;
	case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        m_imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		break;
	case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        m_imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		break;
	case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        m_imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		break;
	default:
		m_imageMemoryBarrier.dstAccessMask = 0;
		break;
	}

    VkPipelineStageFlags srcStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    vkCmdPipelineBarrier(m_commandBuffer->getCmd(0), srcStages, destStages, 0, 0, NULL, 0, NULL, 1, &m_imageMemoryBarrier);
}
