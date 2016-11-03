// vikaApp : main entry point of application
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaApp.h"
#include <vulkan/vulkan.h>

vikaApp::vikaApp(const char *appName, const char *engineName, uint32_t engineVersion, uint32_t appVersion) :
	m_instance(VK_NULL_HANDLE),
	m_res(VK_SUCCESS),
	m_appName(appName),
	m_engineName(engineName),
	m_queueIndex(0),
	m_queuePropCount(0),
	m_devCount(0),
	m_deviceIndex(0),
	m_logicalDevice(nullptr),
	m_surface(nullptr)
{
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

	if (enumeratePhysicalDevices() == false)
	{
		return false;
	}

	return true;
}

// obvious, last method to call to cleanup
void vikaApp::destroy()
{
	if (m_surface != nullptr)
	{
		m_surface->destroy();
		delete m_surface;
		m_surface = nullptr;
	}

	if (m_logicalDevice != nullptr)
	{
		m_logicalDevice->destroy();
		delete m_logicalDevice;
		m_logicalDevice = nullptr;
	}

	if (m_instance != VK_NULL_HANDLE)
	{
	    vkDestroyInstance(m_instance, NULL);
		m_instance = VK_NULL_HANDLE;
	}
}

// also pretty obvious: after initializing, locate a suitable gpu
bool vikaApp::enumeratePhysicalDevices()
{
	// first call: retrieve amount of gpu available
    m_res = vkEnumeratePhysicalDevices(m_instance, &m_devCount, NULL);
	if (m_res != VK_SUCCESS || m_devCount < 1)
	{
		return false;
	}

	m_devices.resize(m_devCount);

	// second call: retrieve actual data of all gpu available (handles)
    m_res = vkEnumeratePhysicalDevices(m_instance, &m_devCount, m_devices.data());
	if (m_res != VK_SUCCESS || m_devCount < 1)
	{
		return false;
	}

	m_deviceProperties.resize(m_devCount);
	m_memoryProperties.resize(m_devCount);
	for (uint32_t i = 0; i < m_devCount; i++)
	{
		VkPhysicalDevice &physDevice = m_devices[i];

	    vkGetPhysicalDeviceMemoryProperties(physDevice, &m_memoryProperties[i]);
		vkGetPhysicalDeviceProperties(physDevice, &m_deviceProperties[i]);
	}
	return true;
}

// TODO: multi-gpu support?
bool vikaApp::createLogicalDevice(uint32_t deviceIndex)
{
	// TODO: overwritable device selection method?
	// -> consider memory, capabilities etc.?
	// this could select some other device if multiple/necessary..
	// assume first is fine for now
	m_deviceIndex = deviceIndex;

	// assume the selected device is fine
	VkPhysicalDevice &physDevice = m_devices[m_deviceIndex];

	// first call: retrieve count
    vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &m_queuePropCount, NULL);
	if (m_queuePropCount < 1)
	{
		return false;
	}

	// remember: proper allocating, don't just mark for capacity
	// when calling direct access to buffer below
	m_queueProperties.resize(m_queuePropCount);

	// second call: retrieve data
    vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &m_queuePropCount, m_queueProperties.data());
	if (m_queuePropCount < 1)
	{
		return false;
	}

	// locate command queue "family" suitable for graphics
	// (might have another queue set for blits with VK_QUEUE_TRANSFER_BIT?)
	// (compute queue support would have VK_QUEUE_COMPUTE_BIT?)
	bool found = false;
	for (uint32_t i = 0; i < m_queueProperties.size(); i++)
	{
		VkQueueFamilyProperties &prop = m_queueProperties[i];
		if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			m_queueIndex = i;
			found = true;
			break;
		}
	}
	if (found == false)
	{
		return false;
	}

	// after checking properties, create logical device from physical device
	m_logicalDevice = new vikaDevice(this, m_queueIndex);
	if (m_logicalDevice->create(physDevice, 1) == false)
	{
		return false;
	}

	return true;
}

// note: physical device needs to be selected before this, do we need logical device too?
// for Win32
bool vikaApp::createSurface(HINSTANCE hInstance, HWND hWnd)
{
	m_surface = new vikaSurface(this);
	if (m_surface->createSurface(hInstance, hWnd) == false)
	{
		return false;
	}

	VkPhysicalDevice &physDevice = m_devices[m_deviceIndex];

	if (m_surface->enumeratePhysDeviceSupport(physDevice, m_queuePropCount, m_queueProperties) == false)
	{
		return false;
	}

	if (m_surface->getFormats(physDevice) == false)
	{
		return false;
	}
	if (m_surface->getCapabilities(physDevice) == false)
	{
		return false;
	}
	if (m_surface->getPresentModes(physDevice) == false)
	{
		return false;
	}
	return true;
}
