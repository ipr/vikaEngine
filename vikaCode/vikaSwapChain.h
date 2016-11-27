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

public: // simplify things..
	uint32_t m_swapchainImageCount;

	//VkPresentModeKHR m_swapchainPresentMode;
	std::vector<uint32_t> m_queueIndices;

    VkSwapchainCreateInfoKHR m_swapchainInfo = {};
	VkSwapchainKHR m_swapchain;

	std::vector<VkImage> m_swapchainImages;
	std::vector<VkImageView> m_swapchainViews;
	std::vector<VkImageViewCreateInfo> m_swapchainCreateViews;


public:
	vikaSwapChain(vikaDevice *logicalDev, vikaSurface *surface);
	virtual ~vikaSwapChain();

	void makeExtentFromCaps(const VkSurfaceCapabilitiesKHR &caps, VkExtent2D &extent) const;

	bool create(VkExtent2D &imageSize);
	void destroy();
};

#endif

