#ifndef _VIKAAPP_H_
#define _VIKAAPP_H_

#pragma once

#include <vulkan/vulkan.h>

class vikaApp
{
protected:
    VkApplicationInfo m_appInfo = {};
    VkInstanceCreateInfo m_instInfo = {};

    VkInstance m_instance;
	VkResult m_res;

public:
	vikaApp(const char *appName);
	~vikaApp();

	VkResult getResult() const { return m_res; };
};

#endif // _VIKAAPP_H_
