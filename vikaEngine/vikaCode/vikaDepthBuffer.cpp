// vikaDepthBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaDepthBuffer.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaDepthBuffer::vikaDepthBuffer(vikaDevice *parent) :
	m_res(VK_SUCCESS),
	m_parent(parent)
{
}

vikaDepthBuffer::~vikaDepthBuffer()
{
	destroy();
}

bool vikaDepthBuffer::create()
{
	return true;
}

void vikaDepthBuffer::destroy()
{
}

