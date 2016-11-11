// vikaDevice : logical device and associated operations for rendering
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaDevice.h"
#include "vikaApp.h"
#include "vikaPhysDevice.h"
#include "vikaSurface.h"

#include <vulkan/vulkan.h>


vikaDevice::vikaDevice(vikaApp *parent, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_parent(parent),
	m_physDevice(physDevice),
	m_device(VK_NULL_HANDLE)
{
	m_queuePriorities = {0.0};
#ifdef _WINDOWS
	m_extensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

    m_queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    m_queueInfo.pNext = NULL;
    m_queueInfo.queueCount = m_queuePriorities.size();
    m_queueInfo.pQueuePriorities = m_queuePriorities.data();

	// note: we need to load platform specific extensions here,
	// for example, Win32 specific surface support

    m_deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    m_deviceInfo.pNext = NULL;
    m_deviceInfo.queueCreateInfoCount = 1;
    m_deviceInfo.pQueueCreateInfos = &m_queueInfo;
    m_deviceInfo.enabledExtensionCount = m_extensionNames.size();
    m_deviceInfo.ppEnabledExtensionNames = NULL;
	if (m_extensionNames.size() > 0)
	{
		m_deviceInfo.ppEnabledExtensionNames = m_extensionNames.data();
	}
    m_deviceInfo.enabledLayerCount = 0;
    m_deviceInfo.ppEnabledLayerNames = NULL;
    m_deviceInfo.pEnabledFeatures = NULL;
}

vikaDevice::~vikaDevice()
{
	destroy();
}

bool vikaDevice::create()
{
	// create logical device from the physical device
    m_res = vkCreateDevice(m_physDevice->getPhysDev(), &m_deviceInfo, NULL, &m_device);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaDevice::destroy()
{
	if (m_device != VK_NULL_HANDLE)
	{
	    vkDestroyDevice(m_device, NULL);
		m_device = VK_NULL_HANDLE;
	}
}

