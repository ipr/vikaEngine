// vikaSurface : platform dependant code and swapchain for surface
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSurface.h"
#include "vikaDevice.h"
#include "vikaApp.h"

#include <vulkan/vulkan.h>


vikaSurface::vikaSurface(vikaDevice *parent, const uint32_t queueIndex) :
	m_res(VK_SUCCESS),
	m_parent(parent),
	m_queueIndex(queueIndex),
	m_surface(VK_NULL_HANDLE)
{
}

vikaSurface::~vikaSurface()
{
	//destroyCommandBuffer();
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

void vikaSurface::destroySurface()
{
}

/*
bool vikaSurface::enumeratePhysDeviceSupport(VkPhysicalDevice &physDevice)
{
	//if (vkGetPhysicalDeviceWin32PresentationSupportKHR(phys, m_queueIndex) == )

	vkGetPhysicalDeviceSurfaceSupportKHR()
}
*/
