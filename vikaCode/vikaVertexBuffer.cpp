// vikaVertexBuffer : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaVertexBuffer.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaVertexBuffer::vikaVertexBuffer(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice)
{
}

vikaVertexBuffer::~vikaVertexBuffer()
{
	destroy();
}

bool vikaVertexBuffer::create()
{
	return true;
}

void vikaVertexBuffer::destroy()
{
}

