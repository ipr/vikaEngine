// vikaSwapChain : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASWAPCHAIN_H_
#define _VIKASWAPCHAIN_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;

class vikaSwapChain
{
protected:
	vikaDevice *m_parent;
	VkResult m_res;

	uint32_t m_swapchainImageCount;

    VkSwapchainCreateInfoKHR m_swapchainInfo = {};
	VkSwapchainKHR m_swapchain;

	std::vector<VkImage> m_swapchainImages;


public:
	vikaSwapChain(vikaDevice *parent, VkSurfaceKHR &surface);
	virtual ~vikaSwapChain();

	bool create();
	void destroy();
};

#endif

