// vikaApp : main entry point of application
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAAPP_H_
#define _VIKAAPP_H_

#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "vikaDevice.h"
#include "vikaSurface.h"

class vikaApp
{
protected:
    VkApplicationInfo m_appInfo = {};
    VkInstanceCreateInfo m_instInfo = {};

    VkInstance m_instance;
	VkResult m_res;

	uint32_t m_queueIndex;
	uint32_t m_queuePropCount;

	std::string m_appName;
	std::string m_engineName;

	std::vector<VkPhysicalDevice> m_devices; // actual gpus
	std::vector<VkPhysicalDeviceProperties> m_deviceProperties;
	std::vector<VkPhysicalDeviceMemoryProperties> m_memoryProperties;
	std::vector<VkQueueFamilyProperties> m_queueProperties;

	// TODO: multi-gpu support?
	uint32_t m_deviceIndex;

	vikaDevice *m_logicalDevice;

	vikaSurface *m_surface;

public:
	vikaApp(const char *appName, const char *engineName, uint32_t engineVersion = 1, uint32_t appVersion = 1);
	virtual ~vikaApp();

	bool create();
	void destroy();

	bool enumeratePhysicalDevices();

	// caller can select different physical device by looking at m_deviceProperties
	//bool setPhysicalDevice(uint32_t deviceIndex = 0) { m_deviceIndex = deviceIndex; };

	bool getDeviceQueueProperties(VkPhysicalDevice &physicalDevice, std::vector<VkQueueFamilyProperties> &props);

	VkResult getResult() const { return m_res; };
	VkInstance& getInstance() { return m_instance; };

	bool createLogicalDevice();
	vikaDevice* getLogicalDevice() { return m_logicalDevice; };

	// for Win32
	bool createSurface(HINSTANCE hInstance, HWND hWnd);
	vikaSurface* getSurface() { return m_surface; };
};

#endif // _VIKAAPP_H_
