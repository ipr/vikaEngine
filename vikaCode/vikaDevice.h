// vikaDevice : logical device and associated operations for rendering
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKADEVICE_H_
#define _VIKADEVICE_H_

#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

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

public:
	vikaDevice(vikaApp *parent, vikaPhysDevice *physDevice, std::vector<const char *> &extensionNames);
	virtual ~vikaDevice();

	bool create();
	void destroy();

	vikaApp *getApp() { return m_parent; };
	VkResult getResult() const { return m_res; };
	VkDevice& getDevice() { return m_device; };
};

#endif // _VIKADEVICE_H_
