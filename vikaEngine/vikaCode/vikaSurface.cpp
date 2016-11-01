// vikaSurface : platform dependant code and swapchain for surface
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSurface.h"
#include "vikaApp.h"

#include <vulkan/vulkan.h>


vikaSurface::vikaSurface(vikaApp *parent) :
	m_res(VK_SUCCESS),
	m_parent(parent),
	m_surface(VK_NULL_HANDLE)
{
}

vikaSurface::~vikaSurface()
{
	destroy();
}

/*
// first platform-dependant thing:
// could make this pure virtual in future
*/

#ifdef _WINDOWS
bool vikaSurface::createSurface(VkInstance &instance, HINSTANCE hInstance, HWND hWnd)
{
    m_srfInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    m_srfInfo.pNext = NULL;
    m_srfInfo.hinstance = hInstance;
    m_srfInfo.hwnd = hWnd;

    m_res = vkCreateWin32SurfaceKHR(instance, 
									&m_srfInfo, NULL, &m_surface);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}
#endif

void vikaSurface::destroy()
{
}

bool vikaSurface::enumeratePhysDeviceSupport(VkPhysicalDevice &physDevice, uint32_t queueCount, std::vector<VkQueueFamilyProperties> &props)
{
	//if (vkGetPhysicalDeviceWin32PresentationSupportKHR(phys, m_queueIndex) == )

	uint32_t graphicsIndex = UINT32_MAX;
	uint32_t presentIndex = UINT32_MAX;

	m_supports.resize(queueCount);
	for (uint32_t i = 0; i < queueCount; i++)
	{
		VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(physDevice, i, m_surface, &m_supports[i]);
	}

	for (uint32_t i = 0; i < queueCount; i++)
	{
		if ((props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && m_supports[i] == VK_TRUE)
		{
			graphicsIndex = i;
			presentIndex = i;
			break;
		}
	}

	if (graphicsIndex == UINT32_MAX || presentIndex == UINT32_MAX)
	{
		return false;
	}
	return true;
}
