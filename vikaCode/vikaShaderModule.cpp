// vikaShaderModule : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaShaderModule.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaShaderModule::vikaShaderModule(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_shader(VK_NULL_HANDLE)
{
	m_shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	m_shaderInfo.pNext = NULL;
	m_shaderInfo.flags = 0;
	m_shaderInfo.codeSize = 0;
	m_shaderInfo.pCode = NULL;
}

vikaShaderModule::~vikaShaderModule()
{
	destroy();
}

bool vikaShaderModule::create()
{
	// TODO: fill in shader code (memory or from file?)
	m_shaderInfo.codeSize = 0;
	m_shaderInfo.pCode = NULL;

	m_res = vkCreateShaderModule(m_logDevice->getDevice(), &m_shaderInfo, NULL, &m_shader);
	if (m_res != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

void vikaShaderModule::destroy()
{
	if (m_shader != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(m_logDevice->getDevice(), m_shader, NULL);
		m_shader = VK_NULL_HANDLE;
	}
}

