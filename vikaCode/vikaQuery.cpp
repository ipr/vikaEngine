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
	m_queryBuffer(nullptr)
{
}

vikaQuery::~vikaQuery()
{
	destroy();
}

bool vikaQuery::create()
{
	return true;
}

void vikaQuery::destroy()
{
	if (m_queryBuffer != nullptr)
	{
		m_queryBuffer->destroy();
		delete m_queryBuffer;
		m_queryBuffer = nullptr;
	}
}

