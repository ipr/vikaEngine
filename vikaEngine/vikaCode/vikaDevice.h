// vikaDevice : logical device and associated operations for rendering
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKADEVICE_H_
#define _VIKADEVICE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "vikaCommandBuffer.h"
#include "vikaSurface.h"


class vikaDevice
{
protected:
    VkDeviceQueueCreateInfo m_queueInfo = {};
    VkDeviceCreateInfo m_deviceInfo = {};

    VkDevice m_device;
	VkResult m_res;

	std::vector<float> m_queuePriorities;
	vikaCommandBuffer m_commandBuffer;
	vikaSurface m_surface;

public:
	vikaDevice(const uint32_t queueIndex);
	~vikaDevice();

	bool create(VkPhysicalDevice &physicalDevice);
	void destroy();

	VkResult getResult() const { return m_res; };
	vikaSurface* getSurface() { return &m_surface; };
};

#endif // _VIKADEVICE_H_
