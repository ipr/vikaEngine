// vikaApp : main entry point of application
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAAPP_H_
#define _VIKAAPP_H_

#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "vikaPhysDevice.h"
#include "vikaDevice.h"
#include "vikaCommandBuffer.h"
#include "vikaDepthBuffer.h"
#include "vikaSurface.h"
#include "vikaSwapChain.h"
#include "vikaUniformBuffer.h"
#include "vikaPipeline.h"
#include "vikaDescriptorset.h"
#include "vikaRenderPass.h"

class vikaApp
{
protected:
    VkApplicationInfo m_appInfo = {};
    VkInstanceCreateInfo m_instInfo = {};

    VkInstance m_instance;
	VkResult m_res;

	std::string m_appName;
	std::string m_engineName;

	std::vector<VkPhysicalDevice> m_devices; // actual gpus

	uint32_t m_devCount; // aka. gpu count

	// TODO: multi-gpu support?
	uint32_t m_deviceIndex;

	vikaPhysDevice *m_physDevice;
	vikaDevice *m_logicalDevice;

	vikaCommandBuffer *m_commandBuffer;
	vikaDepthBuffer *m_depthBuffer;
	vikaSurface *m_surface;
	vikaSwapChain *m_swapChain;
	vikaUniformBuffer *m_uniformBuffer;
	vikaPipeline *m_pipeline;
	vikaDescriptorset *m_descriptorSet;
	vikaRenderPass *m_renderPass;

public:
	vikaApp(const char *appName, const char *engineName, uint32_t engineVersion = 1, uint32_t appVersion = 1);
	virtual ~vikaApp();

	bool create();
	void destroy();

	bool enumeratePhysicalDevices();

	// caller can select different physical device by looking at m_deviceProperties
	//bool setPhysicalDevice(uint32_t deviceIndex = 0) { m_deviceIndex = deviceIndex; };

	VkResult getResult() const { return m_res; };
	VkInstance& getInstance() { return m_instance; };

	// just internal thing for now..
	bool createDevice(uint32_t deviceIndex = 0);
	bool createLogicalDevice(uint32_t cmdBufferCount = 1);
	vikaDevice* getLogicalDevice() { return m_logicalDevice; };

	// note: physical device needs to be selected before this, do we need logical device too?
	// for Win32
	bool createSurface(HINSTANCE hInstance, HWND hWnd);
	vikaSurface* getSurface() { return m_surface; };

	bool createSwapChain();

	//bool createRenderPass();
};

#endif // _VIKAAPP_H_
