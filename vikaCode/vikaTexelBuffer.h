// vikaTexelBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKATEXELBUFFER_H_
#define _VIKATEXELBUFFER_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaPhysDevice;
class vikaBuffer;

class vikaTexelBuffer
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;
	vikaBuffer *m_buffer;

public: // simplify things..
	VkDescriptorBufferInfo m_descInfo = {};

public:
	vikaTexelBuffer(vikaDevice *logDevice, vikaPhysDevice *physDevice);
	virtual ~vikaTexelBuffer();

	bool create(VkDeviceSize bufferSize);
	void destroy();

	bool copyToMemory(uint32_t size, void *data);
};

#endif
