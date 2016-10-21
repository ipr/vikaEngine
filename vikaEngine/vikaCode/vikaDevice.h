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

	uint32_t m_queueIndex;
	std::vector<float> m_queuePriorities;

    VkCommandPoolCreateInfo m_cmdPoolInfo = {};
	VkCommandPool m_cmdPool;
    VkCommandBufferAllocateInfo m_cmdBufferInfo = {};
	VkCommandBuffer m_cmdBuffer;

public:
	vikaDevice(const uint32_t queueIndex);
	~vikaDevice();

	bool create(VkPhysicalDevice &physicalDevice);
	void destroy();

	bool createCommandBuffer();
	void destroyCommandBuffer();

	VkResult getResult() const { return m_res; };
};

#endif // _VIKADEVICE_H_
