// vikaDescriptorset : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaDescriptorset.h"
#include "vikaDevice.h"
#include "vikaPipeline.h"
#include "vikaUniformBuffer.h"

#include <vulkan/vulkan.h>

vikaDescriptorset::vikaDescriptorset(vikaDevice *logDevice, vikaPipeline *pipeline, vikaUniformBuffer *uniBuffer) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_pipeline(pipeline),
	m_uniBuffer(uniBuffer),
	m_pool(VK_NULL_HANDLE)
	//m_setLayout(VK_NULL_HANDLE),
	//m_descriptorset(VK_NULL_HANDLE)
{
}

vikaDescriptorset::~vikaDescriptorset()
{
	destroy();
}

// count of descriptorsets needs to be same in multiple places
bool vikaDescriptorset::create(uint32_t descriptorSetCount)
{
	m_poolsize.resize(1);
	m_poolsize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	m_poolsize[0].descriptorCount = 1;

    m_descriptorPool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    m_descriptorPool.pNext = NULL;
    m_descriptorPool.maxSets = 1;
    m_descriptorPool.poolSizeCount = m_poolsize.size();
    m_descriptorPool.pPoolSizes = m_poolsize.data();

    m_res = vkCreateDescriptorPool(m_logDevice->getDevice(), &m_descriptorPool, NULL, &m_pool);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_allocInfo.resize(1);
    m_allocInfo[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    m_allocInfo[0].pNext = NULL;
    m_allocInfo[0].descriptorPool = m_pool;
    m_allocInfo[0].descriptorSetCount = descriptorSetCount;
    m_allocInfo[0].pSetLayouts = m_pipeline->m_layouts.data();

	m_descriptorset.resize(descriptorSetCount);
    m_res = vkAllocateDescriptorSets(m_logDevice->getDevice(), m_allocInfo.data(), m_descriptorset.data());
	if (m_res != VK_SUCCESS)
	{
		return false;
	}

	m_writeSet.resize(1);
    m_writeSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    m_writeSet[0].pNext = NULL;
    m_writeSet[0].dstSet = m_descriptorset[0]; // just first (of n?)
    m_writeSet[0].descriptorCount = 1;
    m_writeSet[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    m_writeSet[0].pBufferInfo = &info.uniform_data.buffer_info;
    m_writeSet[0].dstArrayElement = 0;
    m_writeSet[0].dstBinding = 0;

    vkUpdateDescriptorSets(m_logDevice->getDevice(), 1, m_writeSet.data(), 0, NULL);

	return true;
}

void vikaDescriptorset::destroy()
{
	if (m_descriptorset.size() > 0)
	{
		vkFreeDescriptorSets(m_logDevice->getDevice(), m_pool, m_descriptorset.size(), m_descriptorset.data());
		m_descriptorset.clear();
	}

	if (m_pool != VK_NULL_HANDLE)
	{
	    vkDestroyDescriptorPool(m_logDevice->getDevice(), m_pool, NULL);
		m_pool = VK_NULL_HANDLE;
	}
}

