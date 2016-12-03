// vikaInstance : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAINSTANCE_H_
#define _VIKAINSTANCE_H_

#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

class vikaInstance
{
protected:
	VkResult m_res;
    VkInstance m_instance;

public:
	// list of layers to be loaded with the instance
	std::vector<const char *> m_layerNames;

	// list of various extensions needed
	std::vector<const char *> m_extensionNames;

	std::string m_appName;
	std::string m_engineName;

    VkApplicationInfo m_appInfo = {};
    VkInstanceCreateInfo m_instInfo = {};

	std::vector<VkPhysicalDevice> m_devices; // actual gpus
	//uint32_t m_devCount; // aka. gpu count

	// layers known to loader
	std::vector<VkLayerProperties> m_layers;

	// extensions supported
	std::vector<VkExtensionProperties> m_instanceExtensions;

public:
	vikaInstance(const char *appName, const char *engineName, uint32_t engineVersion = 1, uint32_t appVersion = 1);
	virtual ~vikaInstance();

	// add needed things before calling create() (which will load them)
	void addLayer(const char *layer)
	{
		m_layerNames.push_back(layer);
	}
	void addExtension(const char *extension)
	{
		m_extensionNames.push_back(extension);
	}

	bool create();
	void destroy();

	bool enumeratePhysicalDevices();
	bool enumerateLayers();
	bool enumerateInstanceExtensions();

	VkResult getResult() const { return m_res; };
	VkInstance& getInstance() { return m_instance; };
	//VkPhysicalDevice& getDevice() { };
};

#endif // 
