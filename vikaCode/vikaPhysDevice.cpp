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
	m_graphicsQueueIndex(0),
	m_presentQueueIndex(0),
	m_physDevice(physDev)
{
}

vikaPhysDevice::~vikaPhysDevice()
{
}

bool vikaPhysDevice::getQueueProperties()
{
	vkGetPhysicalDeviceMemoryProperties(m_physDevice, &m_memoryProperties);

	// includes VkPhysicalDeviceLimits
	vkGetPhysicalDeviceProperties(m_physDevice, &m_deviceProperties);

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
			m_graphicsQueueIndex = i;
			m_presentQueueIndex = i;
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

bool vikaPhysDevice::enumerateDeviceLayers()
{
	uint32_t propCount = 0;
	m_res = vkEnumerateDeviceLayerProperties(m_physDevice, &propCount, NULL);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_layerProperties.resize(propCount);
	m_res = vkEnumerateDeviceLayerProperties(m_physDevice, &propCount, m_layerProperties.data());
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

// depends on: VK_KHR_display (VK_KHR_DISPLAY_EXTENSION_NAME)
// but not available on Windows at least?
// (only some embedded devices?)
bool vikaPhysDevice::enumerateDisplayProperties()
{
	uint32_t propCount = 0;
	m_res = vkGetPhysicalDeviceDisplayPropertiesKHR(m_physDevice, &propCount, NULL);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	if (propCount < 1)
	{
		return true;
	}

	m_displayProperties.resize(propCount);
	m_res = vkGetPhysicalDeviceDisplayPropertiesKHR(m_physDevice, &propCount, m_displayProperties.data());
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

// this method is pretty directly from the API-sample under Apache license
// note: set reqmask to zero if not needed
bool vikaPhysDevice::memtypeBitsToIndex(const VkFlags reqMask, const uint32_t memReqsTypeBits, uint32_t &typeIndex) const
{
	uint32_t memoryTypeBits = memReqsTypeBits;
	for (uint32_t i = 0; i < m_memoryProperties.memoryTypeCount; i++)
	{
		if ((memoryTypeBits & 1) == 1)
		{
			if ((m_memoryProperties.memoryTypes[i].propertyFlags & reqMask) == reqMask)
			{
				typeIndex = i;
				return true;
			}
		}
		memoryTypeBits >>= 1;
	}
	return false;
}

