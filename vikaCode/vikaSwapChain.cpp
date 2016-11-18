// vikaSwapChain : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSwapChain.h"
#include "vikaDevice.h"
#include "vikaSurface.h"

#include <vulkan/vulkan.h>

vikaSwapChain::vikaSwapChain(vikaDevice *logicalDev, vikaSurface *surface, VkExtent2D &imageSize) :
	m_res(VK_SUCCESS),
	m_logicalDev(logicalDev),
	m_surface(surface),
	m_swapchainImageCount(0),
	m_swapchainPresentMode(VK_PRESENT_MODE_FIFO_KHR), // The FIFO present mode is guaranteed by the spec to be supported
	m_swapchainExtent(imageSize), // just init
	m_preTransform(VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR),
	m_swapchain(VK_NULL_HANDLE)
{
	if (surface->m_caps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		m_preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		m_preTransform = surface->m_caps.currentTransform;
	}

	makeExtentFromCaps(surface->m_caps);

    m_swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    m_swapchainInfo.pNext = NULL;
	m_swapchainInfo.surface = m_surface->getSurface();
    m_swapchainInfo.minImageCount = m_surface->m_caps.minImageCount;
    m_swapchainInfo.imageFormat = m_surface->m_format;
	m_swapchainInfo.imageExtent = m_swapchainExtent;
    m_swapchainInfo.preTransform = m_preTransform;
    m_swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    m_swapchainInfo.imageArrayLayers = 1;
    m_swapchainInfo.presentMode = m_swapchainPresentMode;
    m_swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
    m_swapchainInfo.clipped = true;
    m_swapchainInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    m_swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    m_swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    m_swapchainInfo.queueFamilyIndexCount = 0;
    m_swapchainInfo.pQueueFamilyIndices = NULL;

	if (m_surface->m_graphicsQueueIndex != m_surface->m_presentQueueIndex)
	{
		m_queueIndices.push_back(m_surface->m_graphicsQueueIndex);
		m_queueIndices.push_back(m_surface->m_presentQueueIndex);

        // If the graphics and present queues are from different queue families,
        // we either have to explicitly transfer ownership of images between
        // the queues, or we have to create the swapchain with imageSharingMode
        // as VK_SHARING_MODE_CONCURRENT
        m_swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        m_swapchainInfo.queueFamilyIndexCount = m_queueIndices.size();
        m_swapchainInfo.pQueueFamilyIndices = m_queueIndices.data();
	}
}

vikaSwapChain::~vikaSwapChain()
{
	destroy();
}

void vikaSwapChain::makeExtentFromCaps(VkSurfaceCapabilitiesKHR &caps)
{
    // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
    if (caps.currentExtent.width != 0xFFFFFFFF && caps.currentExtent.height != 0xFFFFFFFF) 
	{
        // If the surface size is defined, the swap chain size must match
        m_swapchainExtent = caps.currentExtent;
		return;
	}

    // If the surface size is undefined, the size is set to
    // the size of the images requested.

	// already initialized in construction..
	// note: compare to min and max limits
	if (m_swapchainExtent.width < caps.minImageExtent.width) 
	{
		m_swapchainExtent.width = caps.minImageExtent.width;
	} 
	else if (m_swapchainExtent.width > caps.maxImageExtent.width) 
	{
		m_swapchainExtent.width = caps.maxImageExtent.width;
	}

	if (m_swapchainExtent.height < caps.minImageExtent.height) 
	{
		m_swapchainExtent.height = caps.minImageExtent.height;
	} 
	else if (m_swapchainExtent.height > caps.maxImageExtent.height) 
	{
		m_swapchainExtent.height = caps.maxImageExtent.height;
	}
}

bool vikaSwapChain::create()
{
	// number of presentable images at a time: checking this ensures that can be acquired
	//uint32_t reqImageCount = m_surface->getCaps().minImageCount;

	m_res = vkCreateSwapchainKHR(m_logicalDev->getDevice(), &m_swapchainInfo, NULL, &m_swapchain);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	// get count
	m_res = vkGetSwapchainImagesKHR(m_logicalDev->getDevice(), m_swapchain, &m_swapchainImageCount, NULL);
	if (m_res != VK_SUCCESS || m_swapchainImageCount < 1)
	{
		return false;
	}

	m_swapchainImages.resize(m_swapchainImageCount);
	m_res = vkGetSwapchainImagesKHR(m_logicalDev->getDevice(), m_swapchain, &m_swapchainImageCount, m_swapchainImages.data());
	if (m_res != VK_SUCCESS || m_swapchainImageCount < 1)
	{
		return false;
	}

	/*
	for (uint32_t i = 0; i < m_swapchainImageCount; i++)
	{
		VkImageViewCreateInfo ...;
		vkCreateImageView(..);
	}
	*/

	return true;
}

void vikaSwapChain::destroy()
{
	for (uint32_t i = 0; i < m_swapchainViews.size(); i++)
	{
		// these are destroyed automatically?
		//vkDestroyImage(m_logicalDev->getDevice(), m_swapchainImages[i], NULL);
		vkDestroyImageView(m_logicalDev->getDevice(), m_swapchainViews[i], NULL);
	}
	m_swapchainImages.clear();
	m_swapchainViews.clear();

	if (m_swapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(m_logicalDev->getDevice(), m_swapchain, NULL);
		m_swapchain = VK_NULL_HANDLE;
	}
}
