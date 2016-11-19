// vikaSurface : platform dependant code and swapchain for surface
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASURFACE_H_
#define _VIKASURFACE_H_

#pragma once

#ifdef _WINDOWS
#include <Windows.h>
#endif

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

#ifdef _WINDOWS
	VkWin32SurfaceCreateInfoKHR m_srfInfo = {};
#endif
	VkSurfaceKHR m_surface;

	uint32_t m_graphicsQueueIndex;
	uint32_t m_presentQueueIndex;

	std::vector<VkBool32> m_supports;
	std::vector<VkSurfaceFormatKHR> m_formats;

	uint32_t m_formatCount;
	VkFormat m_format; // format selected

	VkSurfaceCapabilitiesKHR m_caps = {};

	uint32_t m_presentModeCount;
	std::vector<VkPresentModeKHR> m_presents;

public:
	vikaSurface(vikaApp *parent, vikaPhysDevice *physDevice);
	virtual ~vikaSurface();

	/*
	//bool createSurface(VkDevice &device); // TODO: make pure virtual
	bool createSurface(VkInstance &vkInstance, VkDevice &device, HINSTANCE hInstance, HWND hWnd);
	void destroySurface(VkDevice &device);
	*/

#ifdef _WINDOWS
	bool createSurface(HINSTANCE &hInstance, HWND &hWnd);
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

