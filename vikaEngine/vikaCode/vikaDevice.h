// vikaDevice : logical device and associated operations for rendering
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKADEVICE_H_
#define _VIKADEVICE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "vikaCommandBuffer.h"

class vikaApp;

class vikaDevice
{
protected:
    VkDeviceQueueCreateInfo m_queueInfo = {};
    VkDeviceCreateInfo m_deviceInfo = {};

	vikaApp *m_parent;
	VkResult m_res;
    VkDevice m_device;

	std::vector<float> m_queuePriorities;
	vikaCommandBuffer m_commandBuffer;

public:
	vikaDevice(vikaApp *parent, const uint32_t queueIndex);
	~vikaDevice();

	bool create(VkPhysicalDevice &physicalDevice, uint32_t cmdBufferCount = 1);
	void destroy();

	vikaApp *getApp() { return m_parent; };
	VkResult getResult() const { return m_res; };
	VkDevice& getDevice() { return m_device; };
	vikaCommandBuffer& getCommandBuffer() { return m_commandBuffer; };
};

#endif // _VIKADEVICE_H_
