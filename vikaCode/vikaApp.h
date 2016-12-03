// vikaApp : main entry point of application
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAAPP_H_
#define _VIKAAPP_H_

#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "vikaInstance.h"
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
#include "vikaFrameBuffer.h"
#include "vikaVertexBuffer.h"
#include "vikaShaderModule.h"

class vikaApp
{
protected:
	VkResult m_res;

	// size/resolution used in multiple cases
	VkExtent2D m_imageSize = {};

	//std::vector<VkPhysicalDevice> m_devices; // actual gpus
	//uint32_t m_devCount; // aka. gpu count

	// TODO: multi-gpu support?
	uint32_t m_deviceIndex;

	vikaInstance *m_instance;

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
	vikaFrameBuffer *m_framebuffer;
	vikaVertexBuffer *m_vertexBuffer;
	vikaShaderModule *m_shaderModule;

public:
	vikaApp(const char *appName, const char *engineName, uint32_t engineVersion = 1, uint32_t appVersion = 1);
	virtual ~vikaApp();

	// add needed things before calling create() (which will load them)
	/*
	void addLayer(const char *layer)
	{
		m_instance->addLayer(layer);
	}
	void addExtension(const char *extension)
	{
		m_instance->addExtension(extension);
	}
	*/

	// screen/image size
	void setSize(const uint32_t width, const uint32_t height);

	bool create(uint32_t deviceIndex = 0);
	void destroy();

	// caller can select different physical device by looking at m_deviceProperties
	//bool setPhysicalDevice(uint32_t deviceIndex = 0) { m_deviceIndex = deviceIndex; };

	VkResult getResult() const { return m_res; };
	VkInstance& getInstance() { return m_instance->getInstance(); };
	vikaDevice* getLogicalDevice() { return m_logicalDevice; };

	// note: physical device needs to be selected before this, do we need logical device too?
	// for Win32
#ifdef _WINDOWS
	bool createSurface(HINSTANCE &hInstance, HWND &hWnd);
#endif
	vikaSurface* getSurface() { return m_surface; };

	bool createRenderPass(uint32_t cmdBufferCount = 1);

	// TODO: on window size change
	//bool onWindowSize() { return false; };
};

#endif // _VIKAAPP_H_
