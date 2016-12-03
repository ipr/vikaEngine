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
	m_device(VK_NULL_HANDLE),
	m_graphicsQueue(VK_NULL_HANDLE),
	m_presentQueue(VK_NULL_HANDLE)
{
	// range 0..1, 1 highest?
	m_queuePriorities = {1.0};

	// stuff you need later: list of extensions to load
	m_extensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME); // <- available at device level

	m_queueInfo.resize(1);
    m_queueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    m_queueInfo[0].pNext = NULL;
    //m_queueInfo[0].queueCount = m_queuePriorities.size();
    //m_queueInfo[0].pQueuePriorities = m_queuePriorities.data();

    m_deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    m_deviceInfo.pNext = NULL;
    //m_deviceInfo.queueCreateInfoCount = m_queueInfo.size();
    //m_deviceInfo.pQueueCreateInfos = m_queueInfo.data();
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

bool vikaDevice::create(uint32_t graphicsQueueIndex, uint32_t presentQueueIndex)
{
	m_queueInfo[0].queueCount = m_queuePriorities.size();
	m_queueInfo[0].pQueuePriorities = m_queuePriorities.data();

	m_deviceInfo.queueCreateInfoCount = m_queueInfo.size();
	m_deviceInfo.pQueueCreateInfos = m_queueInfo.data();

	// TODO: check that required extensions are supported by the physical device before creating logical device

	// ok, assume list if fine for now
	if (m_extensionNames.size() > 0)
	{
	    m_deviceInfo.enabledExtensionCount = m_extensionNames.size();
		m_deviceInfo.ppEnabledExtensionNames = m_extensionNames.data();
	}
	if (m_layerNames.size() > 0)
	{
		m_deviceInfo.enabledLayerCount = m_layerNames.size();
		m_deviceInfo.ppEnabledLayerNames = m_layerNames.data();
	}

	// create logical device from the physical device
    m_res = vkCreateDevice(m_physDevice->getPhysDev(), &m_deviceInfo, NULL, &m_device);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	// depends on swapchain extension
	vkGetDeviceQueue(m_device, graphicsQueueIndex, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, presentQueueIndex, 0, &m_presentQueue);
	/*
	if (graphicsQueueIndex == presentQueueIndex)
	{
		m_presentQueue = m_graphicsQueue;
	}
	*/
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

// no other logical place for this currently
bool vikaDevice::waitQueueIdle(VkQueue &queue)
{
	// wait completion for infinite timeout
	m_res = vkQueueWaitIdle(queue);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}
