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
    //VkDeviceQueueCreateInfo m_queueInfo = {};
	std::vector<VkDeviceQueueCreateInfo> m_queueInfo;

    VkDeviceCreateInfo m_deviceInfo = {};

	VkResult m_res;
	vikaApp *m_parent;
	vikaPhysDevice *m_physDevice;

    VkDevice m_device; // logical handle

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	std::vector<float> m_queuePriorities;
	std::vector<const char *> m_extensionNames;
	std::vector<const char *> m_layerNames;

public:
	vikaDevice(vikaApp *parent, vikaPhysDevice *physDevice);
	virtual ~vikaDevice();

	// add needed things before calling create() (which will load them)
	void addLayer(const char *layer)
	{
		m_layerNames.push_back(layer);
	}
	void addExtension(const char *extension)
	{
		m_extensionNames.push_back(extension);
	}

	bool create(uint32_t graphicsQueueIndex, uint32_t presentQueueIndex);
	void destroy();

	// no other logical place for this currently
	bool waitQueueIdle(VkQueue &queue);

	vikaApp *getApp() { return m_parent; };
	VkResult getResult() const { return m_res; };
	VkDevice& getDevice() { return m_device; };

	VkQueue& getGraphicsQueue() { return m_graphicsQueue; };
	VkQueue& getPresentQueue() { return m_presentQueue; };
};

#endif // _VIKADEVICE_H_
