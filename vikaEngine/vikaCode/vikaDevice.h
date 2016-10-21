// vikaDevice : logical device and associated operations for rendering
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKADEVICE_H_
#define _VIKADEVICE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>


class vikaDevice
{
protected:
    VkDeviceQueueCreateInfo m_queueInfo = {};
    VkDeviceCreateInfo m_deviceInfo = {};

    VkDevice m_device;
	VkResult m_res;

	std::vector<float> m_queuePriorities;

public:
	vikaDevice();
	~vikaDevice();

	bool create(VkPhysicalDevice &physicalDevice);
	void destroy();

	VkResult getResult() const { return m_res; };
};

#endif // _VIKADEVICE_H_
