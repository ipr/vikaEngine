// vikaQuery : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaQuery.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"
#include "vikaCommandBuffer.h"
#include "vikaBuffer.h"

#include <vulkan/vulkan.h>

vikaQuery::vikaQuery(vikaDevice *logDevice, vikaPhysDevice *physDevice, vikaCommandBuffer *commandBuffer) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice),
	m_commandBuffer(commandBuffer),
	m_queryBuffer(nullptr),
	m_pool(VK_NULL_HANDLE)
{
	m_poolInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
	m_poolInfo.pNext = NULL;
	//m_poolInfo.queryType = VK_QUERY_TYPE_OCCLUSION;
	m_poolInfo.flags = 0;
	//m_poolInfo.queryCount = 2;
	m_poolInfo.pipelineStatistics = 0;

	m_queryBuffer = new vikaBuffer(logDevice, physDevice, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
}

vikaQuery::~vikaQuery()
{
	destroy();
}

bool vikaQuery::create()
{
	uint32_t bufsize = 4 * sizeof(float); // temp
	if (m_queryBuffer->create(bufsize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == false)
	{
		return false;
	}

	m_poolInfo.queryType = VK_QUERY_TYPE_OCCLUSION;
	m_poolInfo.queryCount = 2;

	m_res = vkCreateQueryPool(m_logDevice->getDevice(), &m_poolInfo, NULL, &m_pool);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	vkCmdResetQueryPool(m_commandBuffer->getCmd(0), // same cmd as elsewhere?
						m_pool, 
						0, // first query
						2); // count

	// should be within render pass for rest?
	return true;
}

void vikaQuery::destroy()
{
	if (m_pool != VK_NULL_HANDLE)
	{
		vkDestroyQueryPool(m_logDevice->getDevice(), m_pool, NULL);
		m_pool = VK_NULL_HANDLE;
	}

	if (m_queryBuffer != nullptr)
	{
		m_queryBuffer->destroy();
		delete m_queryBuffer;
		m_queryBuffer = nullptr;
	}
}

