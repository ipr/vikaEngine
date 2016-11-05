// vikaUniformBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaUniformBuffer.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaUniformBuffer::vikaUniformBuffer(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice)
{
}

vikaUniformBuffer::~vikaUniformBuffer()
{
	destroy();
}

// structures not finished yet.. plenty of parameters
bool vikaUniformBuffer::create()
{
	return true;
}

void vikaUniformBuffer::destroy()
{
}

