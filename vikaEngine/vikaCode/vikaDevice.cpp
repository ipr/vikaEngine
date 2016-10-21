// vikaDevice : logical device and associated operations for rendering
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>


vikaDevice::vikaDevice(const uint32_t queueIndex) :
	m_device(VK_NULL_HANDLE),
	m_res(VK_SUCCESS),
	m_queueIndex(queueIndex),
	m_cmdPool(VK_NULL_HANDLE),
	m_cmdBuffer(VK_NULL_HANDLE)
{
	m_queuePriorities.push_back(0.0);

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

bool vikaDevice::create(VkPhysicalDevice &physicalDevice)
{
    m_res = vkCreateDevice(physicalDevice, &m_deviceInfo, NULL, &m_device);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaDevice::destroy()
{
	destroyCommandBuffer();

	if (m_device != VK_NULL_HANDLE)
	{
	    vkDestroyDevice(m_device, NULL);
		m_device = VK_NULL_HANDLE;
	}
}

bool vikaDevice::createCommandBuffer()
{
	m_cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	m_cmdPoolInfo.pNext = NULL;
	m_cmdPoolInfo.queueFamilyIndex = m_queueIndex;
	m_cmdPoolInfo.flags = 0;

	m_res = vkCreateCommandPool(m_device, &m_cmdPoolInfo, NULL, &m_cmdPool);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_cmdBufferInfo.pNext = NULL;
	m_cmdBufferInfo.commandPool = m_cmdPool;
	m_cmdBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_cmdBufferInfo.commandBufferCount = 1;

	m_res = vkAllocateCommandBuffers(m_device, &m_cmdBufferInfo, &m_cmdBuffer);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

void vikaDevice::destroyCommandBuffer()
{
	if (m_cmdBuffer != VK_NULL_HANDLE)
	{
		vkFreeCommandBuffers(m_device, m_cmdPool, m_cmdBufferInfo.commandBufferCount, &m_cmdBuffer);
		m_cmdBuffer = VK_NULL_HANDLE;
	}
	if (m_cmdPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(m_device, m_cmdPool, NULL);
		m_cmdPool = VK_NULL_HANDLE;
	}
}
