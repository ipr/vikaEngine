// vikaSwapChain : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSwapChain.h"
#include "vikaDevice.h"
#include "vikaSurface.h"

#include <vulkan/vulkan.h>

vikaSwapChain::vikaSwapChain(vikaDevice *logicalDev, vikaSurface *surface) :
	m_res(VK_SUCCESS),
	m_logicalDev(logicalDev),
	m_surface(surface),
	m_swapchainImageCount(0),
	m_swapchain(VK_NULL_HANDLE)
{
    m_swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    m_swapchainInfo.pNext = NULL;
	//m_swapchainInfo.surface = m_surface->getSurface(); // filled in later
    //m_swapchainInfo.minImageCount = m_surface->m_caps.minImageCount;
    //m_swapchainInfo.imageFormat = m_surface->m_format;
	//m_swapchainInfo.imageExtent = imageSize; // just init, changed later
    m_swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR; // just init
    m_swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    m_swapchainInfo.imageArrayLayers = 1;
    m_swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // The FIFO present mode is guaranteed by the spec to be supported
    m_swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
    m_swapchainInfo.clipped = true;
    m_swapchainInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    m_swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    m_swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    m_swapchainInfo.queueFamilyIndexCount = 0;
    m_swapchainInfo.pQueueFamilyIndices = NULL;
}

vikaSwapChain::~vikaSwapChain()
{
	destroy();
}

void vikaSwapChain::makeExtentFromCaps(const VkSurfaceCapabilitiesKHR &caps, VkExtent2D &extent) const
{
    // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
    if (caps.currentExtent.width != 0xFFFFFFFF && caps.currentExtent.height != 0xFFFFFFFF) 
	{
        // If the surface size is defined, the swap chain size must match
        extent = caps.currentExtent;
		return;
	}

    // If the surface size is undefined, the size is set to
    // the size of the images requested.

	// already initialized in construction..
	// note: compare to min and max limits
	if (extent.width < caps.minImageExtent.width) 
	{
		extent.width = caps.minImageExtent.width;
	} 
	else if (extent.width > caps.maxImageExtent.width) 
	{
		extent.width = caps.maxImageExtent.width;
	}

	if (extent.height < caps.minImageExtent.height) 
	{
		extent.height = caps.minImageExtent.height;
	} 
	else if (extent.height > caps.maxImageExtent.height) 
	{
		extent.height = caps.maxImageExtent.height;
	}
}

bool vikaSwapChain::create(VkExtent2D &imageSize)
{
	// number of presentable images at a time: checking this ensures that can be acquired
	//uint32_t reqImageCount = m_surface->getCaps().minImageCount;

	m_swapchainInfo.surface = m_surface->getSurface();
    m_swapchainInfo.minImageCount = m_surface->m_caps.minImageCount;
    m_swapchainInfo.imageFormat = m_surface->m_format;

	m_swapchainInfo.imageExtent = imageSize; // just init, changed later
	makeExtentFromCaps(m_surface->m_caps, m_swapchainInfo.imageExtent);

	if (m_surface->m_caps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		m_swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		m_swapchainInfo.preTransform = m_surface->m_caps.currentTransform;
	}

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

	m_swapchainCreateViews.resize(m_swapchainImageCount);
	m_swapchainViews.resize(m_swapchainImageCount);
	for (uint32_t i = 0; i < m_swapchainCreateViews.size(); i++)
	{
		VkImageViewCreateInfo &imageView = m_swapchainCreateViews[i];
		imageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageView.pNext = NULL;
		imageView.flags = 0;
		imageView.image = m_swapchainImages[i];
		imageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageView.format = m_surface->m_format;
		imageView.components.r = VK_COMPONENT_SWIZZLE_R;
		imageView.components.g = VK_COMPONENT_SWIZZLE_G;
		imageView.components.b = VK_COMPONENT_SWIZZLE_B;
		imageView.components.a = VK_COMPONENT_SWIZZLE_A;
		imageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageView.subresourceRange.baseMipLevel = 0;
		imageView.subresourceRange.levelCount = 1;
		imageView.subresourceRange.baseArrayLayer = 0;
		imageView.subresourceRange.layerCount = 1;

		m_res = vkCreateImageView(m_logicalDev->getDevice(), &m_swapchainCreateViews[i], NULL, &m_swapchainViews[i]);
		if (m_res != VK_SUCCESS)
		{
			return false;
		}
	}

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
	m_swapchainCreateViews.clear();

	if (m_swapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(m_logicalDev->getDevice(), m_swapchain, NULL);
		m_swapchain = VK_NULL_HANDLE;
	}
}
