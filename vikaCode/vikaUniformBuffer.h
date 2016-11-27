// vikaUniformBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAUNIFORMBUFFER_H_
#define _VIKAUNIFORMBUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

// no need to include whole glmath lib just for this..
// just 4x4 matrix of floats
struct mat4
{
	float x[4];
	float y[4];
	float z[4];
	float w[4];
};


class vikaDevice;
class vikaPhysDevice;

class vikaUniformBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;

public: // simplify things..
    VkBufferCreateInfo m_bufferInfo = {};
	VkBuffer m_buffer;

	VkDeviceMemory m_devMemory;
	VkMemoryRequirements m_memReqs = {};
	VkMemoryAllocateInfo m_memInfo = {};

	VkDescriptorBufferInfo m_descInfo = {};

public:
	vikaUniformBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice);
	virtual ~vikaUniformBuffer();

	bool create(VkDeviceSize bufferSize);
	void destroy();

	// parameter expected: mat4 with view-projection matrix
	bool copyToMemory(uint32_t sizeMVP, void *dataMVP);
};

#endif
