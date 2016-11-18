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
	m_graphicsIndex(UINT32_MAX),
	m_presentIndex(UINT32_MAX),
	m_formatCount(0),
	m_format(VK_FORMAT_UNDEFINED),
	m_presentModeCount(0)
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
				m_graphicsIndex = i;
				m_presentIndex = i;
			}
		}
	}

	if (m_graphicsIndex == UINT32_MAX || m_presentIndex == UINT32_MAX)
	{
		return false;
	}
	return true;
}

bool vikaSurface::enumerateFormats()
{
	// first call: get count
	m_res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physDevice->getPhysDev(), m_surface, &m_formatCount, NULL);
	if (m_res != VK_SUCCESS || m_formatCount < 1)
	{
		return false;
	}

	m_formats.resize(m_formatCount);

	// second call: get data
	m_res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physDevice->getPhysDev(), m_surface, &m_formatCount, m_formats.data());
	if (m_res != VK_SUCCESS || m_formatCount < 1)
	{
		return false;
	}

	if (m_formatCount == 1 && m_formats[0].format == VK_FORMAT_UNDEFINED)
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
	// first call: get count
	m_res = vkGetPhysicalDeviceSurfacePresentModesKHR(m_physDevice->getPhysDev(), m_surface, &m_presentModeCount, NULL);
	if (m_res != VK_SUCCESS || m_presentModeCount < 1)
	{
		return false;
	}

	m_presents.resize(m_presentModeCount);
	
	// second call: get data
	m_res = vkGetPhysicalDeviceSurfacePresentModesKHR(m_physDevice->getPhysDev(), m_surface, &m_presentModeCount, m_presents.data());
	if (m_res != VK_SUCCESS || m_presentModeCount < 1)
	{
		return false;
	}
	return true;
}

