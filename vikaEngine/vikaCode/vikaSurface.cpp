// vikaSurface : platform dependant code and swapchain for surface
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSurface.h"

#include <vulkan/vulkan.h>


vikaSurface::vikaSurface(const uint32_t queueIndex) :
	m_queueIndex(queueIndex),
	m_cmdPool(VK_NULL_HANDLE),
	m_cmdBuffer(VK_NULL_HANDLE),
	m_surface(VK_NULL_HANDLE)
{
}

vikaSurface::~vikaSurface()
{
	//destroyCommandBuffer();
}

bool vikaSurface::createCommandBuffer(VkDevice &device)
{
	m_cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	m_cmdPoolInfo.pNext = NULL;
	m_cmdPoolInfo.queueFamilyIndex = m_queueIndex;
	m_cmdPoolInfo.flags = 0;

	m_res = vkCreateCommandPool(device, &m_cmdPoolInfo, NULL, &m_cmdPool);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_cmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_cmdBufferInfo.pNext = NULL;
	m_cmdBufferInfo.commandPool = m_cmdPool;
	m_cmdBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_cmdBufferInfo.commandBufferCount = 1;

	m_res = vkAllocateCommandBuffers(device, &m_cmdBufferInfo, &m_cmdBuffer);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

void vikaSurface::destroyCommandBuffer(VkDevice &device)
{
	if (m_cmdBuffer != VK_NULL_HANDLE)
	{
		vkFreeCommandBuffers(device, m_cmdPool, m_cmdBufferInfo.commandBufferCount, &m_cmdBuffer);
		m_cmdBuffer = VK_NULL_HANDLE;
	}
	if (m_cmdPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(device, m_cmdPool, NULL);
		m_cmdPool = VK_NULL_HANDLE;
	}
}

/*
// first platform-dependant thing:
// could make this pure virtual in future
bool vikaSurface::createSurface(VkInstance &vkInstance, VkDevice &device, HINSTANCE hInstance, HWND hWnd)
{
    m_createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    m_createInfo.pNext = NULL;
    m_createInfo.hinstance = hInstance;
    m_createInfo.hwnd = hWnd;

    m_res = vkCreateWin32SurfaceKHR(vkInstance, &m_createInfo, NULL, &m_surface);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaSurface::destroySurface(VkDevice &device)
{
	//vkDestroySwapchainKHR();
}
*/
