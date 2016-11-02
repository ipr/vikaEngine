// vikaSwapChain : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSwapChain.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaSwapChain::vikaSwapChain(vikaDevice *parent) :
	m_res(VK_SUCCESS),
	m_parent(parent),
	m_swapchainImageCount(0),
	m_swapchain(VK_NULL_HANDLE)
{
    m_swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    m_swapchainInfo.pNext = NULL;
}

vikaSwapChain::~vikaSwapChain()
{
	destroy();
}

bool vikaSwapChain::create(VkSurfaceKHR &surface)
{
	m_swapchainInfo.surface = surface;

	m_res = vkCreateSwapchainKHR(m_parent->getDevice(), &m_swapchainInfo, NULL, &m_swapchain);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	// get count
	m_res = vkGetSwapchainImagesKHR(m_parent->getDevice(), m_swapchain, &m_swapchainImageCount, NULL);
	if (m_res != VK_SUCCESS || m_swapchainImageCount < 1)
	{
		return false;
	}

	m_swapchainImages.resize(m_swapchainImageCount);
	m_res = vkGetSwapchainImagesKHR(m_parent->getDevice(), m_swapchain, &m_swapchainImageCount, m_swapchainImages.data());
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
		vkDestroySwapchainKHR(m_parent->getDevice(), m_swapchain, NULL);
		m_swapchain = VK_NULL_HANDLE;
	}
}
