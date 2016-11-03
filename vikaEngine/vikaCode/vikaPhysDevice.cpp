// vikaPhysDevice : manage physical device related properties
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaPhysDevice.h"

#include <vulkan/vulkan.h>

vikaPhysDevice::vikaPhysDevice(uint32_t deviceIndex) :
	m_deviceIndex(deviceIndex)
{
}

vikaPhysDevice::~vikaPhysDevice()
{
}
