// vikaDevice : logical device and associated operations for rendering
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>


vikaDevice::vikaDevice() :
	m_device(VK_NULL_HANDLE),
	m_res(VK_SUCCESS)
{
	m_queuePriorities.push_back(0.0);

    m_queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    m_queueInfo.pNext = NULL;
    m_queueInfo.queueCount = m_queuePriorities.size();
    m_queueInfo.pQueuePriorities = m_queuePriorities.data();

    m_deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    m_deviceInfo.pNext = NULL;
    m_deviceInfo.queueCreateInfoCount = 1;
    m_deviceInfo.pQueueCreateInfos = &m_queueInfo;
    m_deviceInfo.enabledExtensionCount = 0;
    m_deviceInfo.ppEnabledExtensionNames = NULL;
    m_deviceInfo.enabledLayerCount = 0;
    m_deviceInfo.ppEnabledLayerNames = NULL;
    m_deviceInfo.pEnabledFeatures = NULL;
}

vikaDevice::~vikaDevice()
{
	destroy();
}

bool vikaDevice::create(VkPhysicalDevice &physicalDevice)
{
    m_res = vkCreateDevice(physicalDevice, &m_deviceInfo, NULL, &m_device);
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