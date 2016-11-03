// vikaDevice : logical device and associated operations for rendering
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaDevice.h"
#include "vikaApp.h"
#include "vikaPhysDevice.h"

#include <vulkan/vulkan.h>


vikaDevice::vikaDevice(vikaApp *parent, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_parent(parent),
	m_physDevice(physDevice),
	m_device(VK_NULL_HANDLE),
	m_commandBuffer(nullptr)
	//m_depthBuffer(nullptr),
	//m_swapChain(nullptr),
{
	m_queuePriorities = {0.0};

    m_queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    m_queueInfo.pNext = NULL;
    m_queueInfo.queueCount = m_queuePriorities.size();
    m_queueInfo.pQueuePriorities = m_queuePriorities.data();

    m_deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    m_deviceInfo.pNext = NULL;
    m_deviceInfo.queueCreateInfoCount = 1;
    m_deviceInfo.pQueueCreateInfos = &m_queueInfo;
    m_deviceInfo.enabledExtensionCount = 0;
    m_deviceInfo.ppEnabledExtensionNames = NULL;
    m_deviceInfo.enabledLayerCount = 0;
    m_deviceInfo.ppEnabledLayerNames = NULL;
    m_deviceInfo.pEnabledFeatures = NULL;
}

vikaDevice::~vikaDevice()
{
	destroy();
}

bool vikaDevice::create(uint32_t cmdBufferCount)
{
	// create logical device from the physical device
    m_res = vkCreateDevice(m_physDevice->getPhysDev(), &m_deviceInfo, NULL, &m_device);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	// assume one command buffer for now
	m_commandBuffer = new vikaCommandBuffer(this, m_physDevice->getQueueIndex());
	if (m_commandBuffer->create(cmdBufferCount) == false)
	{
		return false;
	}

	/*
	m_swapChain = new vikaSwapChain(this);
	if (m_swapChain->create(m_parent->getSurface()->getSurface()) == false)
	{
		return false;
	}

	m_depthBuffer = new vikaDepthBuffer(this);
	if (m_depthBuffer->create() == false)
	{
		return false;
	}
	*/
	return true;
}

void vikaDevice::destroy()
{
	if (m_device != VK_NULL_HANDLE)
	{
		/*
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
		*/
		if (m_commandBuffer != nullptr)
		{
			m_commandBuffer->destroy();
			delete m_commandBuffer;
			m_commandBuffer = nullptr;
		}

	    vkDestroyDevice(m_device, NULL);
		m_device = VK_NULL_HANDLE;
	}
}
