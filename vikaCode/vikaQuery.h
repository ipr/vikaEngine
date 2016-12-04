// vikaQuery : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKAQUERY_H_
#define _VIKAQUERY_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;
class vikaPhysDevice;
class vikaCommandBuffer;
class vikaBuffer;

class vikaQuery
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;
	vikaPhysDevice *m_physDevice;
	vikaCommandBuffer *m_commandBuffer;
	vikaBuffer *m_queryBuffer;

public: // simplify things..
	VkQueryPoolCreateInfo m_poolInfo = {};
	VkQueryPool m_pool;

public:
	vikaQuery(vikaDevice *logDevice, vikaPhysDevice *physDevice, vikaCommandBuffer *commandBuffer);
	virtual ~vikaQuery();

	bool create();
	void destroy();
};

#endif // 
