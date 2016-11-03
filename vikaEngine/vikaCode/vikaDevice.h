// vikaDevice : logical device and associated operations for rendering
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKADEVICE_H_
#define _VIKADEVICE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "vikaCommandBuffer.h"
#include "vikaDepthBuffer.h"

class vikaApp;
class vikaPhysDevice;

class vikaDevice
{
protected:
    VkDeviceQueueCreateInfo m_queueInfo = {};
    VkDeviceCreateInfo m_deviceInfo = {};

	VkResult m_res;
	vikaApp *m_parent;
	vikaPhysDevice *m_physDevice;

    VkDevice m_device; // logical handle

	std::vector<float> m_queuePriorities;

	vikaCommandBuffer *m_commandBuffer;
	vikaDepthBuffer *m_depthBuffer;

public:
	vikaDevice(vikaApp *parent, vikaPhysDevice *physDevice);
	virtual ~vikaDevice();

	bool create(uint32_t cmdBufferCount = 1);
	void destroy();

	vikaApp *getApp() { return m_parent; };
	VkResult getResult() const { return m_res; };
	VkDevice& getDevice() { return m_device; };
};

#endif // _VIKADEVICE_H_
