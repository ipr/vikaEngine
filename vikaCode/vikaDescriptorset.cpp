// vikaDescriptorset : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaDescriptorset.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaDescriptorset::vikaDescriptorset(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice)
{
	m_poolsize.resize(1);
	m_poolsize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	m_poolsize[0].descriptorCount = 1;
}

vikaDescriptorset::~vikaDescriptorset()
{
	destroy();
}

// structures not finished yet.. plenty of parameters
bool vikaDescriptorset::create()
{
	return true;
}

void vikaDescriptorset::destroy()
{
}

