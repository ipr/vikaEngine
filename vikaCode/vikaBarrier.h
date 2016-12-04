// vikaBarrier : synchronization primitive
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKABARRIER_H_
#define _VIKABARRIER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaGlobalMemoryBarrier
{
protected:
	VkMemoryBarrier m_globalMemoryBarrier = {};

public:
	// global memory barrier
	vikaGlobalMemoryBarrier()
	{
		m_globalMemoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		m_globalMemoryBarrier.pNext = NULL;
		m_globalMemoryBarrier.srcAccessMask = 0;
		m_globalMemoryBarrier.dstAccessMask = 0;
	}
	~vikaGlobalMemoryBarrier()
	{
		destroy();
	}

	void create(VkCommandBuffer &cmdBuf, VkPipelineStageFlags srcStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VkPipelineStageFlags destStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT)
	{
		vkCmdPipelineBarrier(cmdBuf, srcStages, destStages, 0, 1, &m_globalMemoryBarrier, 0, NULL, 0, NULL);
	}
	void destroy()
	{
	}
};

class vikaBufferMemoryBarrier
{
protected:
	VkBufferMemoryBarrier m_bufferMemoryBarrier = {};

public:
	// buffer memory barrier
	vikaBufferMemoryBarrier(VkBuffer &buffer, VkDeviceSize size, VkDeviceSize offset = 0)
	{
		m_bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		m_bufferMemoryBarrier.pNext = NULL;
		m_bufferMemoryBarrier.srcAccessMask = 0;
		m_bufferMemoryBarrier.dstAccessMask = 0;
		m_bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		m_bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		m_bufferMemoryBarrier.buffer = buffer;
		m_bufferMemoryBarrier.offset = offset;
		m_bufferMemoryBarrier.size = size;
	}
	~vikaBufferMemoryBarrier()
	{
		destroy();
	}

	void create(VkCommandBuffer &cmdBuf, VkPipelineStageFlags srcStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VkPipelineStageFlags destStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT)
	{
		vkCmdPipelineBarrier(cmdBuf, srcStages, destStages, 0, 0, NULL, 1, &m_bufferMemoryBarrier, 0, NULL);
	}
	void destroy()
	{
	}
};

class vikaImageMemoryBarrier
{
protected:
	VkImageMemoryBarrier m_imageMemoryBarrier = {};

public:
	// image memory barrier
	vikaImageMemoryBarrier(VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask, VkImage &image)
	{
		m_imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		m_imageMemoryBarrier.pNext = NULL;
		m_imageMemoryBarrier.srcAccessMask = 0;
		m_imageMemoryBarrier.dstAccessMask = 0;
		m_imageMemoryBarrier.oldLayout = oldLayout;
		m_imageMemoryBarrier.newLayout = newLayout;
		m_imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		m_imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		m_imageMemoryBarrier.image = image;
		m_imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
		m_imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
		m_imageMemoryBarrier.subresourceRange.levelCount = 1;
		m_imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
		m_imageMemoryBarrier.subresourceRange.layerCount = 1;

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
	}
	~vikaImageMemoryBarrier()
	{
		destroy();
	}

	void create(VkCommandBuffer &cmdBuf, VkPipelineStageFlags srcStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VkPipelineStageFlags destStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT)
	{
		vkCmdPipelineBarrier(cmdBuf, srcStages, destStages, 0, 0, NULL, 0, NULL, 1, &m_imageMemoryBarrier);
	}
	void destroy()
	{
	}
};

#endif // 
