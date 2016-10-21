// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaApp.h"
#include <vulkan/vulkan.h>

vikaApp::vikaApp(const char *appName, uint32_t appVersion) :
	m_instance(VK_NULL_HANDLE),
	m_res(VK_SUCCESS),
	m_appName(appName),
	m_deviceIndex(0),
	m_queueIndex(0)
{
    m_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    m_appInfo.pNext = NULL;
    m_appInfo.pApplicationName = m_appName.c_str(); // freeform
    m_appInfo.applicationVersion = appVersion;		// freeform
    m_appInfo.pEngineName = m_appName.c_str();		// freeform
    m_appInfo.engineVersion = appVersion;			// freeform
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

vikaApp::~vikaApp()
{
	destroy();
}

// obvious, first method to actually call to initialize:
// only variation is parameters you set in constructor..
bool vikaApp::create()
{
	// in case of failure, no runtime installed?
    m_res = vkCreateInstance(&m_instInfo, NULL, &m_instance);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

// obvious, last method to call to cleanup
void vikaApp::destroy()
{
	if (m_instance != VK_NULL_HANDLE)
	{
	    vkDestroyInstance(m_instance, NULL);
		m_instance = VK_NULL_HANDLE;
	}
}

// also pretty obvious: after initializing, locate a suitable gpu
bool vikaApp::enumeratePhysicalDevices()
{
    uint32_t devCount = 1; // aka. gpu count

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
	m_devices.resize(devCount);

	// this could select some other device if multiple/necessary..
	// assume first is fine for now
	m_deviceIndex = 0;

	// get actual properties of the physical device
	m_deviceProperties.reserve(1);
	vkGetPhysicalDeviceProperties(m_devices[m_deviceIndex], m_deviceProperties.data());
	m_deviceProperties.resize(1);
	return true;
}

// again, pretty obvious: locate properties of devices
bool vikaApp::getDeviceQueueProperties()
{
	// assume the device is fine
	if (getDeviceQueueProperties(m_devices[m_deviceIndex], m_queueProperties) == false)
	{
		return false;
	}

	// locate command queue "family" suitable for graphics
	// (might have another queue set for blits with VK_QUEUE_TRANSFER_BIT?)
	// (compute queue support would have VK_QUEUE_COMPUTE_BIT?)
	auto it = m_queueProperties.begin();
	auto itEnd = m_queueProperties.end();
	while (it != itEnd)
	{
		VkQueueFamilyProperties &prop = (*it);
		if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			//m_queueIndex = i;
			return true;
		}

		++it;
	}
	return false;
}

// again, pretty obvious: locate properties of devices
bool vikaApp::getDeviceQueueProperties(VkPhysicalDevice &physicalDevice, std::vector<VkQueueFamilyProperties> &props)
{
	uint32_t propCount = 1;

	// first call: retrieve count
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, 
											&propCount, 
											NULL);
	if (propCount < 1)
	{
		return false;
	}

	props.reserve(propCount);

	// second call: retrieve data
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, 
											&propCount, 
											props.data());
	if (propCount < 1)
	{
		return false;
	}
	props.resize(propCount);
	return true;
}
