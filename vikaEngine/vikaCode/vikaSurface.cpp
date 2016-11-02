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
	m_surface(VK_NULL_HANDLE),
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
bool vikaSurface::createSurface(VkInstance &instance, HINSTANCE hInstance, HWND hWnd)
{
    m_srfInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    m_srfInfo.pNext = NULL;
	m_srfInfo.flags = 0;
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
	if (m_surface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(m_parent->getInstance(), m_surface, NULL);
		m_surface = VK_NULL_HANDLE;
	}
}

bool vikaSurface::enumeratePhysDeviceSupport(VkPhysicalDevice &physDevice, uint32_t queueCount, const std::vector<VkQueueFamilyProperties> &props)
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

bool vikaSurface::getFormats(VkPhysicalDevice &physDevice)
{
	// first call: get count
	m_res = vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, m_surface, &m_formatCount, NULL);
	if (m_res != VK_SUCCESS || m_formatCount < 1)
	{
		return false;
	}

	m_formats.resize(m_formatCount);

	// second call: get data
	m_res = vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, m_surface, &m_formatCount, m_formats.data());
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

bool vikaSurface::getCapabilities(VkPhysicalDevice &physDevice)
{
	m_res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, m_surface, &m_caps);
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

bool vikaSurface::getPresentModes(VkPhysicalDevice &physDevice)
{
	// first call: get count
	m_res = vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, m_surface, &m_presentModeCount, NULL);
	if (m_res != VK_SUCCESS || m_presentModeCount < 1)
	{
		return false;
	}

	m_presents.resize(m_presentModeCount);
	
	// second call: get data
	m_res = vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, m_surface, &m_presentModeCount, m_presents.data());
	if (m_res != VK_SUCCESS || m_presentModeCount < 1)
	{
		return false;
	}
	return true;
}

