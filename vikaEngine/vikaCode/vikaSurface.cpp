// vikaSurface : platform dependant code and swapchain for surface
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSurface.h"

#include <vulkan/vulkan.h>


vikaSurface::vikaSurface(const uint32_t queueIndex) :
	m_queueIndex(queueIndex),
	m_res(VK_SUCCESS)
{
}

vikaSurface::~vikaSurface()
{
	//destroyCommandBuffer();
}

/*
// first platform-dependant thing:
// could make this pure virtual in future
bool vikaSurface::createSurface(VkInstance &vkInstance, VkDevice &device, HINSTANCE hInstance, HWND hWnd)
{
    m_createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    m_createInfo.pNext = NULL;
    m_createInfo.hinstance = hInstance;
    m_createInfo.hwnd = hWnd;

    m_res = vkCreateWin32SurfaceKHR(vkInstance, &m_createInfo, NULL, &m_surface);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaSurface::destroySurface(VkDevice &device)
{
	//vkDestroySwapchainKHR();
}
*/
