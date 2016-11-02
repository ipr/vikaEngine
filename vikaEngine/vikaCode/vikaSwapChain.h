// vikaSwapChain : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASWAPCHAIN_H_
#define _VIKASWAPCHAIN_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaApp;

class vikaSwapChain
{
protected:
	vikaApp *m_parent;
	VkResult m_res;

public:
	vikaSwapChain(vikaApp *parent);
	virtual ~vikaSwapChain();
};

#endif

