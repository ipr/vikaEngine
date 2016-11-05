// vikaDescriptorset : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaDescriptorset.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaDescriptorset::vikaDescriptorset(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_pool(VK_NULL_HANDLE)
	//m_setLayout(VK_NULL_HANDLE),
	//m_descriptorset(VK_NULL_HANDLE)
{
}

vikaDescriptorset::~vikaDescriptorset()
{
	destroy();
}

// structures not finished yet.. plenty of parameters
bool vikaDescriptorset::create()
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

	/*
	m_allocInfo.resize(1);
    m_allocInfo[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    m_allocInfo[0].pNext = NULL;
    m_allocInfo[0].descriptorPool = m_pool;
    m_allocInfo[0].descriptorSetCount = m_descriptorPool.maxSets;
    m_allocInfo[0].pSetLayouts = m_setLayout;

    m_res = vkAllocateDescriptorSets(m_logDevice->getDevice(), m_allocInfo.data(), &m_descriptorset);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	*/

	return true;
}

void vikaDescriptorset::destroy()
{
	if (m_pool != VK_NULL_HANDLE)
	{
	    vkDestroyDescriptorPool(m_logDevice->getDevice(), m_pool, NULL);
		m_pool = VK_NULL_HANDLE;
	}
}

