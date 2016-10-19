#include "stdafx.h"

#include <vulkan/vulkan.h>

HINSTANCE g_hInst;                                // current instance

const char *pShortName = "vikaEngine";

int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	g_hInst = hInstance;

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = pShortName;
    appInfo.applicationVersion = 1;
    appInfo.pEngineName = pShortName;
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instInfo = {};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pNext = NULL;
    instInfo.flags = 0;
    instInfo.pApplicationInfo = &appInfo;
    instInfo.enabledExtensionCount = 0;
    instInfo.ppEnabledExtensionNames = NULL;
    instInfo.enabledLayerCount = 0;
    instInfo.ppEnabledLayerNames = NULL;


	return 0;
}

