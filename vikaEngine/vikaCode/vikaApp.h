// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAAPP_H_
#define _VIKAAPP_H_

#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

class vikaApp
{
protected:
    VkApplicationInfo m_appInfo = {};
    VkInstanceCreateInfo m_instInfo = {};

    VkInstance m_instance;
	VkResult m_res;

	// TODO: multi-gpu support?
	size_t m_deviceIndex;

	std::string m_appName;
	std::vector<VkPhysicalDevice> m_devices;
	std::vector<VkQueueFamilyProperties> m_properties;

public:
	vikaApp(const char *appName, uint32_t appVersion = 1);
	~vikaApp();

	bool create();
	void destroy();

	bool enumerateDevices();
	bool getDeviceProperties();
	bool getDeviceProperties(VkPhysicalDevice &physicalDevice, std::vector<VkQueueFamilyProperties> &props);

	VkResult getResult() const { return m_res; };
};

#endif // _VIKAAPP_H_
