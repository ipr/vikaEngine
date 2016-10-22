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

class vikaApp
{
protected:
    VkApplicationInfo m_appInfo = {};
    VkInstanceCreateInfo m_instInfo = {};

    VkInstance m_instance;
	VkResult m_res;

	uint32_t m_queueIndex;

	std::string m_appName;
	std::vector<VkPhysicalDevice> m_devices;
	std::vector<VkQueueFamilyProperties> m_queueProperties;

	// TODO: multi-gpu support?
	size_t m_deviceIndex;

	VkPhysicalDeviceProperties m_devProp = {};
	vikaDevice *m_logicalDevice;

public:
	vikaApp(const char *appName, uint32_t appVersion = 1);
	~vikaApp();

	bool create();
	void destroy();

	bool enumeratePhysicalDevices();
	bool getDeviceQueueProperties();
	bool getDeviceQueueProperties(VkPhysicalDevice &physicalDevice, std::vector<VkQueueFamilyProperties> &props);

	VkResult getResult() const { return m_res; };

	bool prepareLogicalDevice();
	vikaDevice* getLogicalDevice() { return m_logicalDevice; };
};

#endif // _VIKAAPP_H_
