// vikaSwapChain : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASWAPCHAIN_H_
#define _VIKASWAPCHAIN_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaSurface;

class vikaSwapChain
{
protected:
	VkResult m_res;
	vikaDevice *m_logicalDev;
	vikaSurface *m_surface;

	uint32_t m_swapchainImageCount;

	VkPresentModeKHR m_swapchainPresentMode;
	//VkExtent2D m_swapchainExtent = {};

    VkSwapchainCreateInfoKHR m_swapchainInfo = {};
	VkSwapchainKHR m_swapchain;

	std::vector<VkImage> m_swapchainImages;


public:
	vikaSwapChain(vikaDevice *logicalDev, vikaSurface *surface);
	virtual ~vikaSwapChain();

	bool create();
	void destroy();
};

#endif
