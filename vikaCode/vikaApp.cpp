// vikaApp : main entry point of application
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaApp.h"
#include <vulkan/vulkan.h>

vikaApp::vikaApp(const char *appName, const char *engineName, uint32_t engineVersion, uint32_t appVersion) :
	m_instance(VK_NULL_HANDLE),
	m_res(VK_SUCCESS),
	m_appName(appName),
	m_engineName(engineName),
	m_devCount(0),
	m_deviceIndex(0),
	m_physDevice(nullptr),
	m_logicalDevice(nullptr),
	m_commandBuffer(nullptr),
	m_depthBuffer(nullptr),
	m_surface(nullptr),
	m_swapChain(nullptr),
	m_uniformBuffer(nullptr),
	m_pipeline(nullptr),
	m_descriptorSet(nullptr),
	m_renderPass(nullptr)
{
	// stuff you need later: list of extensions to load
	m_extensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME); // <- available at instance level
#ifdef _WINDOWS
	// available at instance level, but must be listed after "generic" surface extension? (driver bug?)
	// other thing: add system env variable VK_LAYER_PATH if this extension is not working (might be another thing)
	m_extensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME); 
#endif

	m_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	m_appInfo.pNext = NULL;
	m_appInfo.pApplicationName = m_appName.c_str(); // freeform
	m_appInfo.applicationVersion = appVersion;		// freeform
	m_appInfo.pEngineName = m_engineName.c_str();		// freeform
	m_appInfo.engineVersion = engineVersion;			// freeform
	m_appInfo.apiVersion = VK_API_VERSION_1_0;

	m_instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	m_instInfo.pNext = NULL;
	m_instInfo.flags = 0;
	m_instInfo.pApplicationInfo = &m_appInfo;
	m_instInfo.enabledExtensionCount = 0;
	m_instInfo.ppEnabledExtensionNames = NULL;
	m_instInfo.enabledLayerCount = 0;
	m_instInfo.ppEnabledLayerNames = NULL;
}

vikaApp::~vikaApp()
{
	destroy();
}

// obvious, first method to actually call to initialize:
// only variation is parameters you set in constructor..
bool vikaApp::create()
{
	// check supported extensions before trying to create instance
	if (enumerateInstanceExtensions() == false)
	{
		return false;
	}

	// TODO: check that required extensions are supported

	// ok, assume list if fine for now
	if (m_extensionNames.size() > 0)
	{
		m_instInfo.enabledExtensionCount = m_extensionNames.size();
		m_instInfo.ppEnabledExtensionNames = m_extensionNames.data();
	}

	// Note! must load list of layers to get win32 surface extension working properly?
	// driver bug possibly?
	if (enumerateLayers() == false)
	{
		return false;
	}

	if (m_layerNames.size() > 0)
	{
		m_instInfo.enabledLayerCount = m_layerNames.size();
		m_instInfo.ppEnabledLayerNames = m_layerNames.data();
	}

	// in case of failure, no runtime installed?
    m_res = vkCreateInstance(&m_instInfo, NULL, &m_instance);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	if (enumeratePhysicalDevices() == false)
	{
		return false;
	}

	// potentially could be left to whoever is calling this
	if (createDevice() == false)
	{
		return false;
	}
	return true;
}

// obvious, last method to call to cleanup
void vikaApp::destroy()
{
	if (m_renderPass != nullptr)
	{
		m_renderPass->destroy();
		delete m_renderPass;
		m_renderPass = nullptr;
	}

	if (m_descriptorSet != nullptr)
	{
		m_descriptorSet->destroy();
		delete m_descriptorSet;
		m_descriptorSet = nullptr;
	}
	if (m_pipeline != nullptr)
	{
		m_pipeline->destroy();
		delete m_pipeline;
		m_pipeline = nullptr;
	}
	if (m_uniformBuffer != nullptr)
	{
		m_uniformBuffer->destroy();
		delete m_uniformBuffer;
		m_uniformBuffer = nullptr;
	}

	if (m_swapChain != nullptr)
	{
		m_swapChain->destroy();
		delete m_swapChain;
		m_swapChain = nullptr;
	}

	if (m_surface != nullptr)
	{
		m_surface->destroy();
		delete m_surface;
		m_surface = nullptr;
	}

	if (m_depthBuffer != nullptr)
	{
		m_depthBuffer->destroy();
		delete m_depthBuffer;
		m_depthBuffer = nullptr;
	}
	if (m_commandBuffer != nullptr)
	{
		m_commandBuffer->destroy();
		delete m_commandBuffer;
		m_commandBuffer = nullptr;
	}

	if (m_logicalDevice != nullptr)
	{
		m_logicalDevice->destroy();
		delete m_logicalDevice;
		m_logicalDevice = nullptr;
	}

	if (m_physDevice != nullptr)
	{
		//m_physDevice->destroy();
		delete m_physDevice;
		m_physDevice = nullptr;
	}

	if (m_instance != VK_NULL_HANDLE)
	{
	    vkDestroyInstance(m_instance, NULL);
		m_instance = VK_NULL_HANDLE;
	}
}

// enumerate layers known to loader
bool vikaApp::enumerateLayers()
{
	uint32_t layerCount = 0;
	m_res = vkEnumerateInstanceLayerProperties(&layerCount, NULL);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_layers.resize(layerCount);
	m_res = vkEnumerateInstanceLayerProperties(&layerCount, m_layers.data());
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

// also pretty obvious: after initializing, locate a suitable gpu
bool vikaApp::enumeratePhysicalDevices()
{
	// first call: retrieve amount of gpu available
    m_res = vkEnumeratePhysicalDevices(m_instance, &m_devCount, NULL);
	if (m_res != VK_SUCCESS || m_devCount < 1)
	{
		return false;
	}

	m_devices.resize(m_devCount);

	// second call: retrieve actual data of all gpu available (handles)
    m_res = vkEnumeratePhysicalDevices(m_instance, &m_devCount, m_devices.data());
	if (m_res != VK_SUCCESS || m_devCount < 1)
	{
		return false;
	}
	return true;
}

bool vikaApp::enumerateInstanceExtensions()
{
	uint32_t extensionsCount = 0;

	// no layer name, first call: get count
	m_res = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, NULL);
	if (m_res != VK_SUCCESS || extensionsCount < 1)
	{
		return false;
	}

	m_instanceExtensions.resize(extensionsCount);

	// no layer name, second call: get data
	m_res = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, m_instanceExtensions.data());
	if (m_res != VK_SUCCESS || extensionsCount < 1)
	{
		return false;
	}
	return true;
}

// just internal thing for now..
// TODO: multi-gpu support?
bool vikaApp::createDevice(uint32_t deviceIndex)
{
	// TODO: overwritable device selection method?
	// -> consider memory, capabilities etc.?
	// this could select some other device if multiple/necessary..
	// assume first is fine for now
	m_deviceIndex = deviceIndex;
	// assume the selected device is fine
	VkPhysicalDevice &physDevice = m_devices[m_deviceIndex];

	m_physDevice = new vikaPhysDevice(this, physDevice, deviceIndex);
	m_physDevice->getPhysProperties();
	m_physDevice->getQueueProperties();
	m_physDevice->enumerateDeviceExtensions();
	m_physDevice->enumerateDeviceLayers();

	return createLogicalDevice(1);
}

// TODO: multi-gpu support?
bool vikaApp::createLogicalDevice(uint32_t cmdBufferCount)
{
	if (m_physDevice == nullptr)
	{
		return false;
	}

	// after checking properties, create logical device from physical device
	m_logicalDevice = new vikaDevice(this, m_physDevice);
	if (m_logicalDevice->create() == false)
	{
		return false;
	}

	// assume one command buffer for now
	m_commandBuffer = new vikaCommandBuffer(m_logicalDevice, m_physDevice->getQueueIndex());
	if (m_commandBuffer->create(cmdBufferCount) == false)
	{
		return false;
	}

	/* not working yet
	m_depthBuffer = new vikaDepthBuffer(m_logicalDevice, m_physDevice);
	if (m_depthBuffer->create() == false)
	{
		return false;
	}
	*/

	return true;
}

// note: physical device needs to be selected before this, do we need logical device too?
// for Win32
bool vikaApp::createSurface(HINSTANCE &hInstance, HWND &hWnd)
{
	if (m_physDevice == nullptr)
	{
		return false;
	}

	m_surface = new vikaSurface(this, m_physDevice);
	if (m_surface->createSurface(hInstance, hWnd) == false)
	{
		return false;
	}

	/* not working yet
	m_swapChain = new vikaSwapChain(m_logicalDevice, m_surface);
	if (m_swapChain->create() == false)
	{
		return false;
	}
	*/

	return true;
}

/*
bool vikaApp::createSwapChain()
{
}
*/

/*
bool vikaApp::createRenderPass()
{
	m_renderPass = new vikaRenderPass(m_logicalDevice);
}
*/