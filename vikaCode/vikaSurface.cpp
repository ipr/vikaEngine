// vikaSurface : platform dependant code and swapchain for surface
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSurface.h"
#include "vikaApp.h"
#include "vikaPhysDevice.h"

#include <vulkan/vulkan.h>


vikaSurface::vikaSurface(vikaApp *parent, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_parent(parent),
	m_physDevice(physDevice),
	m_surface(VK_NULL_HANDLE),
	m_graphicsQueueIndex(UINT32_MAX),
	m_presentQueueIndex(UINT32_MAX),
	m_format(VK_FORMAT_UNDEFINED)
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
// note: needs extension loaded,
// VK_KHR_SWAPCHAIN_EXTENSION_NAME
// VK_KHR_WIN32_SURFACE_EXTENSION_NAME
bool vikaSurface::createSurface(HINSTANCE &hInstance, HWND &hWnd)
{
    m_srfInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    m_srfInfo.pNext = NULL;
	m_srfInfo.flags = 0;
    m_srfInfo.hinstance = hInstance;
    m_srfInfo.hwnd = hWnd;

	// note: must load appropriate extension to vulkan before this can be used
    m_res = vkCreateWin32SurfaceKHR(m_parent->getInstance(), &m_srfInfo, NULL, &m_surface);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return createInternals();
}
#endif

bool vikaSurface::createInternals()
{
	if (enumeratePhysDeviceSupport() == false)
	{
		return false;
	}
	if (enumerateFormats() == false)
	{
		return false;
	}
	if (enumerateCapabilities() == false)
	{
		return false;
	}
	if (enumeratePresentModes() == false)
	{
		return false;
	}
	return true;
}

void vikaSurface::destroy()
{
	if (m_surface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(m_parent->getInstance(), m_surface, NULL);
		m_surface = VK_NULL_HANDLE;
	}
}

bool vikaSurface::enumeratePhysDeviceSupport()
{
	//if (vkGetPhysicalDeviceWin32PresentationSupportKHR(phys, m_queueIndex) == )

	m_supports.resize(m_physDevice->m_queuePropCount);
	for (uint32_t i = 0; i < m_physDevice->m_queuePropCount; i++)
	{
		VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(m_physDevice->getPhysDev(), i, m_surface, &m_supports[i]);
		if (res == VK_SUCCESS)
		{
			if ((m_physDevice->m_queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && m_supports[i] == VK_TRUE)
			{
				m_graphicsQueueIndex = i;
				m_presentQueueIndex = i;
				return true;
			}
		}
	}

	// TODO: check if additional logic is necessary here..

	if (m_graphicsQueueIndex == UINT32_MAX || m_presentQueueIndex == UINT32_MAX)
	{
		return false;
	}
	return true;
}

bool vikaSurface::enumerateFormats()
{
	uint32_t formatCount = 0;
	m_res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physDevice->getPhysDev(), m_surface, &formatCount, NULL);
	if (m_res != VK_SUCCESS || formatCount < 1)
	{
		return false;
	}

	m_formats.resize(formatCount);
	m_res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physDevice->getPhysDev(), m_surface, &formatCount, m_formats.data());
	if (m_res != VK_SUCCESS || formatCount < 1)
	{
		return false;
	}

	if (formatCount == 1 && m_formats[0].format == VK_FORMAT_UNDEFINED)
	{
		// no preferred format defined -> select something
		m_format = VK_FORMAT_B8G8R8A8_UNORM;
	}
	else
	{
		// just select first one
		m_format = m_formats[0].format;
	}
	return true;
}

bool vikaSurface::enumerateCapabilities()
{
	m_res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physDevice->getPhysDev(), m_surface, &m_caps);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	/*
    // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
    if (m_caps.currentExtent.width == 0xFFFFFFFF)
	{}
	*/

	return true;
}

bool vikaSurface::enumeratePresentModes()
{
	uint32_t presentModeCount = 0;
	m_res = vkGetPhysicalDeviceSurfacePresentModesKHR(m_physDevice->getPhysDev(), m_surface, &presentModeCount, NULL);
	if (m_res != VK_SUCCESS || presentModeCount < 1)
	{
		return false;
	}

	m_presentModes.resize(presentModeCount);
	m_res = vkGetPhysicalDeviceSurfacePresentModesKHR(m_physDevice->getPhysDev(), m_surface, &presentModeCount, m_presentModes.data());
	if (m_res != VK_SUCCESS || presentModeCount < 1)
	{
		return false;
	}
	return true;
}

