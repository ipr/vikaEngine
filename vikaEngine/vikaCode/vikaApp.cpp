#include "stdafx.h"
#include "vikaApp.h"
#include <vulkan/vulkan.h>

vikaApp::vikaApp(const char *appName)
{
    m_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    m_appInfo.pNext = NULL;
    m_appInfo.pApplicationName = appName;
    m_appInfo.applicationVersion = 1;
    m_appInfo.pEngineName = appName;
    m_appInfo.engineVersion = 1;
    m_appInfo.apiVersion = VK_API_VERSION_1_0;

    m_instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    m_instInfo.pNext = NULL;
    m_instInfo.flags = 0;
    m_instInfo.pApplicationInfo = &m_appInfo;
    m_instInfo.enabledExtensionCount = 0;
    m_instInfo.ppEnabledExtensionNames = NULL;
    m_instInfo.enabledLayerCount = 0;
    m_instInfo.ppEnabledLayerNames = NULL;

    m_res = vkCreateInstance(&m_instInfo, NULL, &m_instance);
    if (m_res == VK_ERROR_INCOMPATIBLE_DRIVER) 
	{}
}


vikaApp::~vikaApp()
{
    vkDestroyInstance(m_instance, NULL);
}
