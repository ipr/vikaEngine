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
	m_stageName("main"),
	m_shader(VK_NULL_HANDLE)
{
    m_shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_shaderStage.pNext = NULL;
    m_shaderStage.pSpecializationInfo = NULL;
    m_shaderStage.flags = 0;
    m_shaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
    m_shaderStage.pName = m_stageName.c_str();

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

// VK_SHADER_STAGE_VERTEX_BIT or VK_SHADER_STAGE_FRAGMENT_BIT, "main"
bool vikaShaderModule::addStage(VkShaderStageFlagBits stage, const char *name, const char *shaderText)
{
    m_shaderStage.stage = stage; // VK_SHADER_STAGE_VERTEX_BIT or VK_SHADER_STAGE_FRAGMENT_BIT
	m_stageName = name;
    m_shaderStage.pName = m_stageName.c_str();

	// TODO: compile GLSL to SPV 
    //GLSLtoSPV(VK_SHADER_STAGE_VERTEX_BIT, shaderText, m_spirv);

	m_shaderInfo.codeSize = m_spirv.size() * sizeof(unsigned int);
	m_shaderInfo.pCode = m_spirv.data();

	return true;
}

bool vikaShaderModule::addStage(VkShaderStageFlagBits stage, const char *name, unsigned int *spirv, size_t size)
{
    m_shaderStage.stage = stage; // VK_SHADER_STAGE_VERTEX_BIT or VK_SHADER_STAGE_FRAGMENT_BIT
	m_stageName = name;
    m_shaderStage.pName = m_stageName.c_str();

	// use given spirv code as-is
	m_spirv.resize(size);
	memcpy(m_spirv.data(), spirv, size * sizeof(unsigned int));

	m_shaderInfo.codeSize = m_spirv.size() * sizeof(unsigned int);
	m_shaderInfo.pCode = m_spirv.data();

	return true;
}

bool vikaShaderModule::create()
{
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

