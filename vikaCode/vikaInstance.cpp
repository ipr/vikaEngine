// vikaInstance : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaInstance.h"

#include <vulkan/vulkan.h>

vikaInstance::vikaInstance(const char *appName, const char *engineName, uint32_t engineVersion, uint32_t appVersion) :
	m_instance(VK_NULL_HANDLE),
	m_res(VK_SUCCESS),
	m_appName(appName),
	m_engineName(engineName)
{
	// stuff you need later: list of extensions to load
	m_extensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME); // <- available at instance level
#ifdef VK_USE_PLATFORM_WIN32_KHR
	// available at instance level, but must be listed after "generic" surface extension? (driver bug?)
	// other thing: add system env variable VK_LAYER_PATH if this extension is not working (might be another thing)
	m_extensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME); 
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
	m_extensionNames.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#endif
#ifdef VK_USE_PLATFORM_MIR_KHR
	m_extensionNames.push_back(VK_KHR_MIR_SURFACE_EXTENSION_NAME);
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
	m_extensionNames.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
	m_extensionNames.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif

	m_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	m_appInfo.pNext = NULL;
	m_appInfo.pApplicationName = m_appName.c_str(); // freeform
	m_appInfo.applicationVersion = appVersion;		// freeform
	m_appInfo.pEngineName = m_engineName.c_str();		// freeform
	m_appInfo.engineVersion = engineVersion;			// freeform
	m_appInfo.apiVersion = VK_API_VERSION_1_0;

	m_instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	m_instInfo.pNext = NULL;
	m_instInfo.flags = 0;
	m_instInfo.pApplicationInfo = &m_appInfo;
	m_instInfo.enabledExtensionCount = 0;
	m_instInfo.ppEnabledExtensionNames = NULL;
	m_instInfo.enabledLayerCount = 0;
	m_instInfo.ppEnabledLayerNames = NULL;
}

vikaInstance::~vikaInstance()
{
	destroy();
}

bool vikaInstance::create()
{
	// TODO: check that required extensions are supported

	// check supported extensions before trying to create instance
	if (enumerateInstanceExtensions() == false)
	{
		return false;
	}

	// Note! must load list of layers to get win32 surface extension working properly?
	// driver bug possibly?
	if (enumerateLayers() == false)
	{
		return false;
	}

	if (m_layerNames.size() > 0)
	{
		m_instInfo.enabledLayerCount = m_layerNames.size();
		m_instInfo.ppEnabledLayerNames = m_layerNames.data();
	}
	if (m_extensionNames.size() > 0)
	{
		m_instInfo.enabledExtensionCount = m_extensionNames.size();
		m_instInfo.ppEnabledExtensionNames = m_extensionNames.data();
	}

	// in case of failure, no runtime installed?
    m_res = vkCreateInstance(&m_instInfo, NULL, &m_instance);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return enumeratePhysicalDevices();
}

void vikaInstance::destroy()
{
	if (m_instance != VK_NULL_HANDLE)
	{
	    vkDestroyInstance(m_instance, NULL);
		m_instance = VK_NULL_HANDLE;
	}
}

// after initializing, locate a suitable gpu
bool vikaInstance::enumeratePhysicalDevices()
{
	uint32_t devCount = 0;

	// first call: retrieve amount of gpu available
    m_res = vkEnumeratePhysicalDevices(m_instance, &devCount, NULL);
	if (m_res != VK_SUCCESS || devCount < 1)
	{
		return false;
	}

	m_devices.resize(devCount);

	// second call: retrieve actual data of all gpu available (handles)
    m_res = vkEnumeratePhysicalDevices(m_instance, &devCount, m_devices.data());
	if (m_res != VK_SUCCESS || devCount < 1)
	{
		return false;
	}
	return true;
}

// enumerate layers known to loader
bool vikaInstance::enumerateLayers()
{
	uint32_t layerCount = 0;
	m_res = vkEnumerateInstanceLayerProperties(&layerCount, NULL);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_layers.resize(layerCount);
	m_res = vkEnumerateInstanceLayerProperties(&layerCount, m_layers.data());
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

bool vikaInstance::enumerateInstanceExtensions()
{
	uint32_t extensionsCount = 0;
	m_res = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, NULL);
	if (m_res != VK_SUCCESS || extensionsCount < 1)
	{
		return false;
	}

	m_instanceExtensions.resize(extensionsCount);
	m_res = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, m_instanceExtensions.data());
	if (m_res != VK_SUCCESS || extensionsCount < 1)
	{
		return false;
	}
	return true;
}

