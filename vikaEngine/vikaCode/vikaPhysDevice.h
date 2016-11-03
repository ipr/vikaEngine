// vikaPhysDevice : manage physical device related properties
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAPHYSDEVICE_H_
#define _VIKAPHYSDEVICE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaApp;

class vikaPhysDevice
{
protected:
	VkResult m_res;
	vikaApp *m_parent;

public: // make things simpler..
	uint32_t m_deviceIndex;
	uint32_t m_queuePropCount;
	uint32_t m_queueIndex;

	VkPhysicalDevice m_physDevice;

	VkPhysicalDeviceProperties m_deviceProperties = {};
	VkPhysicalDeviceMemoryProperties m_memoryProperties = {};
	std::vector<VkQueueFamilyProperties> m_queueProperties;

public:
	vikaPhysDevice(vikaApp *parent, VkPhysicalDevice &physDev, uint32_t deviceIndex);
	~vikaPhysDevice();

	bool getPhysProperties();
	bool getQueueProperties();

	uint32_t getQueueIndex() const { return m_queueIndex; };
	VkPhysicalDevice& getPhysDev() { return m_physDevice; }; 
};

#endif
