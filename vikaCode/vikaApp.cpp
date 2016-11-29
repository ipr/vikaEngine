// vikaApp : main entry point of application
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaApp.h"
#include <vulkan/vulkan.h>

vikaInstance::vikaInstance(const char *appName, const char *engineName, uint32_t engineVersion, uint32_t appVersion) :
	m_instance(VK_NULL_HANDLE),
	m_res(VK_SUCCESS),
	m_appName(appName),
	m_engineName(engineName)
{
	// stuff you need later: list of extensions to load
	m_extensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME); // <- available at instance level
#ifdef VK_USE_PLATFORM_WIN32_KHR
	// available at instance level, but must be listed after "generic" surface extension? (driver bug?)
	// other thing: add system env variable VK_LAYER_PATH if this extension is not working (might be another thing)
	m_extensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME); 
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
	m_extensionNames.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#endif
#ifdef VK_USE_PLATFORM_MIR_KHR
	m_extensionNames.push_back(VK_KHR_MIR_SURFACE_EXTENSION_NAME);
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
	m_extensionNames.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
	m_extensionNames.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
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

vikaInstance::~vikaInstance()
{
	destroy();
}

bool vikaInstance::create()
{
	// TODO: check that required extensions are supported

	// check supported extensions before trying to create instance
	if (enumerateInstanceExtensions() == false)
	{
		return false;
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
	if (m_extensionNames.size() > 0)
	{
		m_instInfo.enabledExtensionCount = m_extensionNames.size();
		m_instInfo.ppEnabledExtensionNames = m_extensionNames.data();
	}

	// in case of failure, no runtime installed?
    m_res = vkCreateInstance(&m_instInfo, NULL, &m_instance);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return enumeratePhysicalDevices();
}

void vikaInstance::destroy()
{
	if (m_instance != VK_NULL_HANDLE)
	{
	    vkDestroyInstance(m_instance, NULL);
		m_instance = VK_NULL_HANDLE;
	}
}

// after initializing, locate a suitable gpu
bool vikaInstance::enumeratePhysicalDevices()
{
	uint32_t devCount = 0;

	// first call: retrieve amount of gpu available
    m_res = vkEnumeratePhysicalDevices(m_instance, &devCount, NULL);
	if (m_res != VK_SUCCESS || devCount < 1)
	{
		return false;
	}

	m_devices.resize(devCount);

	// second call: retrieve actual data of all gpu available (handles)
    m_res = vkEnumeratePhysicalDevices(m_instance, &devCount, m_devices.data());
	if (m_res != VK_SUCCESS || devCount < 1)
	{
		return false;
	}
	return true;
}

// enumerate layers known to loader
bool vikaInstance::enumerateLayers()
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

bool vikaInstance::enumerateInstanceExtensions()
{
	uint32_t extensionsCount = 0;
	m_res = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, NULL);
	if (m_res != VK_SUCCESS || extensionsCount < 1)
	{
		return false;
	}

	m_instanceExtensions.resize(extensionsCount);
	m_res = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, m_instanceExtensions.data());
	if (m_res != VK_SUCCESS || extensionsCount < 1)
	{
		return false;
	}
	return true;
}

//////////////////////

vikaApp::vikaApp(const char *appName, const char *engineName, uint32_t engineVersion, uint32_t appVersion) :
	m_res(VK_SUCCESS),
	m_deviceIndex(0),
	m_instance(nullptr),
	m_physDevice(nullptr),
	m_logicalDevice(nullptr),
	m_commandBuffer(nullptr),
	m_depthBuffer(nullptr),
	m_surface(nullptr),
	m_swapChain(nullptr),
	m_uniformBuffer(nullptr),
	m_pipeline(nullptr),
	m_descriptorSet(nullptr),
	m_renderPass(nullptr),
	m_framebuffer(nullptr),
	m_vertexBuffer(nullptr),
	m_shaderModule(nullptr)
{
	m_instance = new vikaInstance(appName, engineName, engineVersion, appVersion);
}

vikaApp::~vikaApp()
{
	destroy();
}

// screen/image size,
// used in multiple places
void vikaApp::setSize(const uint32_t width, const uint32_t height)
{
	m_imageSize.width = width;
	m_imageSize.height = height;
}

// TODO: improve device selection method, multi-GPU support?
bool vikaApp::create(uint32_t deviceIndex)
{
	if (m_instance->create() == false)
	{
		return false;
	}

	// TODO: overwritable device selection method?
	// -> consider memory, capabilities etc.?
	// this could select some other device if multiple/necessary..
	// assume first is fine for now
	m_deviceIndex = deviceIndex;

	// assume the selected device is fine
	m_physDevice = new vikaPhysDevice(this, m_instance->m_devices[m_deviceIndex], deviceIndex);
	if (m_physDevice->getQueueProperties() == false)
	{
		// not suitable for graphics?
		return false;
	}
	m_physDevice->enumerateDeviceExtensions();
	m_physDevice->enumerateDeviceLayers();
	return true;
}

// obvious, last method to call to cleanup
void vikaApp::destroy()
{
	if (m_shaderModule != nullptr)
	{
		m_shaderModule->destroy();
		delete m_shaderModule;
		m_shaderModule = nullptr;
	}

	if (m_vertexBuffer != nullptr)
	{
		m_vertexBuffer->destroy();
		delete m_vertexBuffer;
		m_vertexBuffer = nullptr;
	}
	if (m_framebuffer != nullptr)
	{
		m_framebuffer->destroy();
		delete m_framebuffer;
		m_framebuffer = nullptr;
	}
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

	if (m_surface != nullptr)
	{
		m_surface->destroy();
		delete m_surface;
		m_surface = nullptr;
	}
	if (m_physDevice != nullptr)
	{
		//m_physDevice->destroy();
		delete m_physDevice;
		m_physDevice = nullptr;
	}
	if (m_instance != nullptr)
	{
		m_instance->destroy();
		delete m_instance;
		m_instance = nullptr;
	}
}


// note: physical device needs to be selected before this, 
// also create logical device from physical device
// since that will load the necessary extension(s) for us needed in creating surface
// (otherwise vkCreateWin32SurfaceKHR() won't exist and will crash)
//
// for Win32
#ifdef _WINDOWS
bool vikaApp::createSurface(HINSTANCE &hInstance, HWND &hWnd)
{
	// note: extensions VK_KHR_SURFACE_EXTENSION_NAME and VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	// must be loaded first on instance-level for surface creation to work here

	if (m_physDevice == nullptr)
	{
		return false;
	}

	m_surface = new vikaSurface(this, m_physDevice);
	if (m_surface->createSurface(hInstance, hWnd) == false)
	{
		return false;
	}

	return true;
}
#endif

bool vikaApp::createRenderPass(uint32_t cmdBufferCount)
{
	// after checking properties, create logical device from physical device
	m_logicalDevice = new vikaDevice(this, m_physDevice);
	if (m_logicalDevice->create(m_physDevice->m_graphicsQueueIndex, m_physDevice->m_presentQueueIndex) == false)
	{
		return false;
	}

	// assume one command buffer for now
	m_commandBuffer = new vikaCommandBuffer(m_logicalDevice);
	if (m_commandBuffer->create(m_physDevice->m_graphicsQueueIndex, cmdBufferCount) == false)
	{
		return false;
	}

	// TODO: execute here?
	m_commandBuffer->executeBegin();

	m_swapChain = new vikaSwapChain(m_logicalDevice, m_surface);
	if (m_swapChain->create(m_imageSize) == false)
	{
		return false;
	}

	m_depthBuffer = new vikaDepthBuffer(m_logicalDevice, m_physDevice);
	if (m_depthBuffer->create(m_imageSize, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_D16_UNORM) == false)
	{
		return false;
	}

	// TODO: fill in view-projection matrix
	mat4 MVP;
	memset(&MVP.x, 0, sizeof(MVP.x));
	memset(&MVP.y, 0, sizeof(MVP.y));
	memset(&MVP.z, 0, sizeof(MVP.z));
	memset(&MVP.w, 0, sizeof(MVP.w));
	m_uniformBuffer = new vikaUniformBuffer(m_logicalDevice, m_physDevice);
	if (m_uniformBuffer->create(sizeof(mat4)) == false)
	{
		return false;
	}
	if (m_uniformBuffer->copyToMemory(sizeof(mat4), &MVP) == false)
	{
		return false;
	}

	m_pipeline = new vikaPipeline(m_logicalDevice);
	if (m_pipeline->createLayout() == false)
	{
		return false;
	}
	
	m_descriptorSet = new vikaDescriptorset(m_logicalDevice, m_pipeline, m_uniformBuffer);
	if (m_descriptorSet->create() == false)
	{
		return false;
	}

	m_framebuffer = new vikaFrameBuffer(m_logicalDevice, m_depthBuffer, m_swapChain);
	m_vertexBuffer = new vikaVertexBuffer(m_logicalDevice, m_physDevice, m_commandBuffer);
	m_renderPass = new vikaRenderPass(m_logicalDevice, m_surface, m_swapChain, m_commandBuffer, m_depthBuffer, m_framebuffer, m_vertexBuffer, m_pipeline, m_descriptorSet);
	if (m_renderPass->create(VK_SAMPLE_COUNT_1_BIT) == false)
	{
		return false;
	}
	if (m_framebuffer->create(m_renderPass, m_imageSize) == false)
	{
		return false;
	}

	uint64_t vertexDataSize = 0;
	uint32_t vertexStrideSize = 0;
	if (m_vertexBuffer->create(vertexDataSize, vertexStrideSize) == false)
	{
		return false;
	}
	//if (m_vertexBuffer->copyToMemory() == false)

	m_pipeline->setVertexBuffer(m_vertexBuffer);
	//m_pipeline->setShaders(m_shaderModule);

	if (m_renderPass->beginPass(VK_SUBPASS_CONTENTS_INLINE) == true)
	{
		// must be within render pass to bind vertex buffer?
		m_renderPass->bindVertexBuffer();
		m_renderPass->bindPipeline();
		m_renderPass->bindDescriptorSets();
		m_renderPass->endPass();
	}

	if (m_pipeline->createInputAssembly(m_renderPass, VK_SAMPLE_COUNT_1_BIT, 1, 1) == false)
	{
		return false;
	}

	// TODO: execute "end" here?
	m_commandBuffer->executeEnd();
	m_commandBuffer->executeQueue();
	return true;
}

