// vikaPipeline : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaPipeline.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaPipeline::vikaPipeline(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice)
{
}

vikaPipeline::~vikaPipeline()
{
	destroy();
}

// structures not finished yet.. plenty of parameters
bool vikaPipeline::create()
{
	return true;
}

void vikaPipeline::destroy()
{
}

