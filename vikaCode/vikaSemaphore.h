// vikaSemaphore : synchronization primitive as reusable class
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASEMAPHORE_H_
#define _VIKASEMAPHORE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaSemaphore
{
protected:
	VkResult m_res;
	VkSemaphore m_semaphore;
	VkDevice m_device;

	VkSemaphoreCreateInfo m_semaphoreInfo = {};

public:
	vikaSemaphore(VkDevice &device) : 
		m_res(VK_SUCCESS),
		m_semaphore(VK_NULL_HANDLE),
		m_device(device)
	{
		m_semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		m_semaphoreInfo.pNext = NULL;
		m_semaphoreInfo.flags = 0;
	}
	~vikaSemaphore() 
	{
		destroy();
	};
	bool create()
	{
	    m_res = vkCreateSemaphore(m_device, &m_semaphoreInfo, NULL, &m_semaphore);
		if (m_res != VK_SUCCESS)
		{
			return false;
		}
		return true;
	}
	void destroy()
	{
		if (m_semaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(m_device, m_semaphore, NULL);
			m_semaphore = VK_NULL_HANDLE;
		}
	}

	VkResult getResult() const { return m_res; };
	VkSemaphore& getSemaphore() { return m_semaphore; };
};

#endif // 
