// vikaSurface : platform dependant code and swapchain for surface
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASURFACE_H_
#define _VIKASURFACE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaApp;
class vikaPhysDevice;

class vikaSurface
{
protected:
	VkResult m_res;
	vikaApp *m_parent;
	vikaPhysDevice *m_physDevice;

// VK_USE_PLATFORM_WIN32_KHR
	// TODO: add in derived class
    //VkWin32SurfaceCreateInfoKHR m_createInfo;

public: // make things simpler..
	std::vector<const char *> m_requiredExtensions;

#ifdef VK_USE_PLATFORM_WIN32_KHR
	VkWin32SurfaceCreateInfoKHR m_srfInfo = {};
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
	VkWaylandSurfaceCreateInfoKHR m_srfInfo = {};
#endif
#ifdef VK_USE_PLATFORM_MIR_KHR
	VkMirSurfaceCreateInfoKHR m_srfInfo = {};
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
	VkXcbSurfaceCreateInfoKHR m_srfInfo = {};
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
	VkXlibSurfaceCreateInfoKHR m_srfInfo = {};
#endif

	VkSurfaceKHR m_surface;
	//VkBool32 m_physDevPresentSupport;

	// TODO: should be members of physical device?
	uint32_t m_graphicsQueueIndex;
	uint32_t m_presentQueueIndex;

	std::vector<VkBool32> m_supports;
	std::vector<VkSurfaceFormatKHR> m_formats;

	VkFormat m_format; // format selected

	VkSurfaceCapabilitiesKHR m_caps = {};

	std::vector<VkPresentModeKHR> m_presentModes;

public:
	vikaSurface(vikaApp *parent, vikaPhysDevice *physDevice);
	virtual ~vikaSurface();

	/*
	//bool createSurface(VkDevice &device); // TODO: make pure virtual
	bool createSurface(VkInstance &vkInstance, VkDevice &device, HINSTANCE hInstance, HWND hWnd);
	void destroySurface(VkDevice &device);
	*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
	bool createSurface(HINSTANCE &hInstance, HWND &hWnd);
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
	bool createSurface(struct wl_display *display, struct wl_surface *surface);
#endif
#ifdef VK_USE_PLATFORM_MIR_KHR
	bool createSurface(MirConnection *connection, MirSurface *mirSurface);
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
	bool createSurface(xcb_connection_t *connection, xcb_window_t window);
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
	bool createSurface(Display *dpy, Window window);
#endif

	bool createInternals();
	void destroy();

	bool enumeratePhysDeviceSupport();

	bool enumerateFormats();
	bool enumerateCapabilities();
	bool enumeratePresentModes();

	VkResult getResult() const { return m_res; };
	VkSurfaceKHR& getSurface() { return m_surface; };
	//VkSurfaceCapabilitiesKHR& getCaps() { return m_caps; };
};

#endif // _VIKASURFACE_H_

