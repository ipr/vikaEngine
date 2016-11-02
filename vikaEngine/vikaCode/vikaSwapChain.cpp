// vikaSwapChain : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSwapChain.h"
#include "vikaApp.h"

#include <vulkan/vulkan.h>

vikaSwapChain::vikaSwapChain(vikaApp *parent) :
	m_res(VK_SUCCESS),
	m_parent(parent)
{
}

vikaSwapChain::~vikaSwapChain()
{
	//destroy();
}

