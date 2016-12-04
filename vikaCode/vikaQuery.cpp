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

bool vikaQuery::create(uint32_t cmdBufferIndex)
{
	uint32_t bufsize = 4 * sizeof(uint64_t); // temp
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

	vkCmdResetQueryPool(m_commandBuffer->getCmd(cmdBufferIndex), // same cmd as elsewhere?
						m_pool, 
						0, // first query
						m_poolInfo.queryCount); // count

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

void vikaQuery::beginQuery(uint32_t slot, uint32_t cmdBufferIndex)
{
	vkCmdBeginQuery(m_commandBuffer->getCmd(cmdBufferIndex), 
					m_pool, 
					slot, // slot
					0); // flags
}

void vikaQuery::endQuery(uint32_t slot, uint32_t cmdBufferIndex)
{
	vkCmdEndQuery(m_commandBuffer->getCmd(cmdBufferIndex), 
				m_pool, 
				slot); // slot
}

void vikaQuery::copyResults(uint32_t cmdBufferIndex)
{
	vkCmdCopyQueryPoolResults(m_commandBuffer->getCmd(cmdBufferIndex),
							m_pool,
							0, // first query
							m_poolInfo.queryCount, // count
							m_queryBuffer->m_buffer,
							0, // dst offset
							sizeof(uint64_t), // stride
							VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT);
}

bool vikaQuery::getResults()
{
	uint64_t samples_passed[4];
    samples_passed[0] = 0;
    samples_passed[1] = 0;

	m_res = vkGetQueryPoolResults(m_logDevice->getDevice(), 
								m_pool, 
								0, // first query
								m_poolInfo.queryCount, // count
								4 * sizeof(uint64_t),
								samples_passed,
								sizeof(uint64_t), // stride
								VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	// read back results from buffer
	uint64_t *mapping = (uint64_t*)m_queryBuffer->mapMem();
	if (mapping == nullptr)
	{
		return false;
	}

	uint64_t res1 = mapping[0];
	uint64_t res2 = mapping[1];

	m_queryBuffer->unmapMem();
	return true;
}
