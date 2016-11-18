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
	VkExtent2D m_swapchainExtent = {};

	VkSurfaceTransformFlagBitsKHR m_preTransform;

	std::vector<uint32_t> m_queueIndices;

    VkSwapchainCreateInfoKHR m_swapchainInfo = {};
	VkSwapchainKHR m_swapchain;

	std::vector<VkImage> m_swapchainImages;


public:
	vikaSwapChain(vikaDevice *logicalDev, vikaSurface *surface, VkExtent2D &imageSize);
	virtual ~vikaSwapChain();

	void makeExtentFromCaps(VkSurfaceCapabilitiesKHR &caps);

	bool create();
	void destroy();
};

#endif

