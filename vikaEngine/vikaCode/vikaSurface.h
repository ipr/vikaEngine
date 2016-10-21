// vikaSurface : platform dependant code and swapchain for surface
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASURFACE_H_
#define _VIKASURFACE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>


class vikaSurface
{
protected:
	VkResult m_res;

	uint32_t m_queueIndex;

    VkCommandPoolCreateInfo m_cmdPoolInfo = {};
	VkCommandPool m_cmdPool;
    VkCommandBufferAllocateInfo m_cmdBufferInfo = {};
	VkCommandBuffer m_cmdBuffer;

public:
	vikaSurface(const uint32_t queueIndex);
	~vikaSurface();

	bool createCommandBuffer(VkDevice &device);
	void destroyCommandBuffer(VkDevice &device);

	VkResult getResult() const { return m_res; };
};

#endif // _VIKASURFACE_H_

