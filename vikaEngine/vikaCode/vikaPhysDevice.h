// vikaPhysDevice : manage physical device related properties
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAPHYSDEVICE_H_
#define _VIKAPHYSDEVICE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaPhysDevice
{
protected:
	uint32_t m_deviceIndex;

	std::vector<VkPhysicalDeviceProperties> m_deviceProperties;
	std::vector<VkPhysicalDeviceMemoryProperties> m_memoryProperties;
	std::vector<VkQueueFamilyProperties> m_queueProperties;

public:
	vikaPhysDevice(uint32_t deviceIndex);
	~vikaPhysDevice();
};

#endif
