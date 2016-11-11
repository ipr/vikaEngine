// vikaPhysDevice : manage physical device related properties
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaPhysDevice.h"
#include "vikaApp.h"
#include <vulkan/vulkan.h>

vikaPhysDevice::vikaPhysDevice(vikaApp *parent, VkPhysicalDevice &physDev, uint32_t deviceIndex) :
	m_res(VK_SUCCESS),
	m_parent(parent),
	m_deviceIndex(deviceIndex),
	m_queuePropCount(0),
	m_queueIndex(0),
	m_physDevice(physDev)
{
}

vikaPhysDevice::~vikaPhysDevice()
{
}

bool vikaPhysDevice::getPhysProperties()
{
	vkGetPhysicalDeviceMemoryProperties(m_physDevice, &m_memoryProperties);
	vkGetPhysicalDeviceProperties(m_physDevice, &m_deviceProperties);

	return true;
}

bool vikaPhysDevice::getQueueProperties()
{
	// first call: retrieve count
    vkGetPhysicalDeviceQueueFamilyProperties(m_physDevice, &m_queuePropCount, NULL);
	if (m_queuePropCount < 1)
	{
		return false;
	}

	// remember: proper allocating, don't just mark for capacity
	// when calling direct access to buffer below
	m_queueProperties.resize(m_queuePropCount);

	// second call: retrieve data
    vkGetPhysicalDeviceQueueFamilyProperties(m_physDevice, &m_queuePropCount, m_queueProperties.data());
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
	return true;
}

bool vikaPhysDevice::enumerateDeviceExtensions()
{
	uint32_t extensionCount = 0;

	// no layer name, first call: get count
	m_res = vkEnumerateDeviceExtensionProperties(m_physDevice, nullptr, &extensionCount, NULL);
	if (m_res != VK_SUCCESS || extensionCount < 1)
	{
		return false;
	}

	m_extensionProperties.resize(extensionCount);

	m_res = vkEnumerateDeviceExtensionProperties(m_physDevice, nullptr, &extensionCount, m_extensionProperties.data());
	if (m_res != VK_SUCCESS || extensionCount < 1)
	{
		return false;
	}
	return true;


}
