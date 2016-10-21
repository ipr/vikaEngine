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

	std::string m_appName;
	std::vector<VkPhysicalDevice> m_devices;

public:
	vikaApp(const char *appName);
	~vikaApp();

	bool create();
	void destroy();

	bool enumerateDevices();

	VkResult getResult() const { return m_res; };
};

#endif // _VIKAAPP_H_
