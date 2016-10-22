// vikaSurface : platform dependant code and swapchain for surface
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASURFACE_H_
#define _VIKASURFACE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>


class vikaSurface
{
protected:
	VkResult m_res;

	uint32_t m_queueIndex;

// VK_USE_PLATFORM_WIN32_KHR
	// TODO: add in derived class
    //VkWin32SurfaceCreateInfoKHR m_createInfo;

	VkSurfaceKHR m_surface;

public:
	vikaSurface(const uint32_t queueIndex);
	~vikaSurface();

	/*
	//bool createSurface(VkDevice &device); // TODO: make pure virtual
	bool createSurface(VkInstance &vkInstance, VkDevice &device, HINSTANCE hInstance, HWND hWnd);
	void destroySurface(VkDevice &device);
	*/

	VkResult getResult() const { return m_res; };
};

#endif // _VIKASURFACE_H_

