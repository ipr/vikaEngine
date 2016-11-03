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
	m_swapchainPresentMode(VK_PRESENT_MODE_FIFO_KHR), // The FIFO present mode is guaranteed by the spec to be supported
	m_swapchain(VK_NULL_HANDLE)
{
	//m_swapchainExtent = m_surface->getCaps().minImageExtent;

    m_swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    m_swapchainInfo.pNext = NULL;
	m_swapchainInfo.surface = m_surface->getSurface();
	m_swapchainInfo.imageExtent = m_surface->getCaps().minImageExtent;
}

vikaSwapChain::~vikaSwapChain()
{
	destroy();
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

	return true;
}

void vikaSwapChain::destroy()
{
	if (m_swapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(m_logicalDev->getDevice(), m_swapchain, NULL);
		m_swapchain = VK_NULL_HANDLE;
	}
}
