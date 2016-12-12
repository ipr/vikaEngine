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
	uint32_t m_graphicsQueueIndex;
	uint32_t m_presentQueueIndex;

	VkPhysicalDevice m_physDevice;

	// see also: VkPhysicalDeviceLimits
	VkPhysicalDeviceProperties m_deviceProperties = {};

	VkPhysicalDeviceMemoryProperties m_memoryProperties = {};
	std::vector<VkQueueFamilyProperties> m_queueProperties;

	std::vector<VkExtensionProperties> m_extensionProperties;
	std::vector<VkLayerProperties> m_layerProperties;

	std::vector<VkDisplayPropertiesKHR> m_displayProperties;

	// TODO: separate handling for display modes etc.
	//std::vector<VkDisplayPlanePropertiesKHR> m_displayPlaneProperties;
	//std::vector<VkDisplayModePropertiesKHR> m_displayModeProperties;

public:
	vikaPhysDevice(vikaApp *parent, VkPhysicalDevice &physDev, uint32_t deviceIndex);
	~vikaPhysDevice();

	bool getQueueProperties();
	bool enumerateDeviceExtensions();
	bool enumerateDeviceLayers();

	/*
	bool enumerateDisplayProperties();
	*/

	bool memtypeBitsToIndex(const VkFlags reqMask, const uint32_t memReqsTypeBits, uint32_t &typeIndex) const;

	//uint32_t getQueueIndex() const { return m_graphicsQueueIndex; };
	VkPhysicalDevice& getPhysDev() { return m_physDevice; }; 
};

#endif
