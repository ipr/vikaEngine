// vikaFence : synchronization primitive as reusable class
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAFENCE_H_
#define _VIKAFENCE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaFence
{
protected:
	VkResult m_res;
	VkFence m_fence;
	VkDevice m_device;

	// Amount of time, in nanoseconds, to wait
	uint64_t m_fenceTimeout;

	VkFenceCreateInfo m_fenceInfo = {};

public:
	vikaFence(VkDevice &device, uint64_t timeOut = 100000000) : 
		m_res(VK_SUCCESS),
		m_fence(VK_NULL_HANDLE),
		m_device(device),
		m_fenceTimeout(timeOut)
	{
		m_fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		m_fenceInfo.pNext = NULL;
		m_fenceInfo.flags = 0;
	}
	~vikaFence() 
	{
		destroy();
	};
	bool create()
	{
	    m_res = vkCreateFence(m_device, &m_fenceInfo, NULL, &m_fence);
		if (m_res != VK_SUCCESS)
		{
			return false;
		}
		return true;
	}
	void destroy()
	{
		if (m_fence != VK_NULL_HANDLE)
		{
		    vkDestroyFence(m_device, m_fence, NULL);
			m_fence = VK_NULL_HANDLE;
		}
	}

	bool doWait()
	{
		do 
		{
			m_res = vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, m_fenceTimeout);
		} while (m_res == VK_TIMEOUT);

		if (m_res != VK_SUCCESS)
		{
			return false;
		}
		return true;
	}

	VkResult getResult() const { return m_res; };
	VkFence& getFence() { return m_fence; };
};

#endif // 
