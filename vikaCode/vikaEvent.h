// vikaEvent : synchronization primitive as reusable class
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAEVENT_H_
#define _VIKAEVENT_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaEvent
{
protected:
	VkResult m_res;
	VkEvent m_event;
	VkDevice m_device;

	VkEventCreateInfo m_eventInfo = {};

public:
	vikaEvent(VkDevice &device) : 
		m_res(VK_SUCCESS),
		m_event(VK_NULL_HANDLE),
		m_device(device)
	{
		m_eventInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
		m_eventInfo.pNext = NULL;
		m_eventInfo.flags = 0;
	}
	~vikaEvent() 
	{
		destroy();
	};
	bool create()
	{
	    m_res = vkCreateEvent(m_device, &m_eventInfo, NULL, &m_event);
		if (m_res != VK_SUCCESS)
		{
			return false;
		}
		return true;
	}
	void destroy()
	{
		if (m_event != VK_NULL_HANDLE)
		{
		    vkDestroyEvent(m_device, m_event, NULL);
			m_event = VK_NULL_HANDLE;
		}
	}

	/*
	bool isSet()
	{
		VKResult status = vkGetEventStatus(m_device, m_event);
	}
	bool setEvent()
	{}
	bool resetEvent()
	{}
	*/

	/*
	bool doWait()
	{
		do 
		{
		} while (m_res == VK_TIMEOUT);

		if (m_res != VK_SUCCESS)
		{
			return false;
		}
		return true;
	}
	*/

	VkResult getResult() const { return m_res; };
	VkEvent& getEvent() { return m_event; };
};

#endif // 
