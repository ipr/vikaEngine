// vikaShaderModule : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaShaderModule.h"
#include "vikaDevice.h"

#include <vulkan/vulkan.h>

vikaShaderModule::vikaShaderModule(vikaDevice *logDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice)
	//m_stageName("main")
	//m_shader(VK_NULL_HANDLE)
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

// VK_SHADER_STAGE_VERTEX_BIT or VK_SHADER_STAGE_FRAGMENT_BIT, "main"
/*
bool vikaShaderModule::addStage(VkShaderStageFlagBits stage, const char *name, const char *shaderText)
{
	std::vector<unsigned int> spirvTmp;
	// TODO: compile GLSL to SPV 
	//GLSLtoSPV(VK_SHADER_STAGE_VERTEX_BIT, shaderText, spirvTmp);

	return addStage(stage, name, spirvTmp.data(), spirvTmp.size());
}
*/

// VK_SHADER_STAGE_VERTEX_BIT or VK_SHADER_STAGE_FRAGMENT_BIT, "main"
bool vikaShaderModule::addStage(VkShaderStageFlagBits stage, const char *name, unsigned int *spirv, size_t size)
{
	m_shaderStages.resize(m_shaderStages.size() +1);
	VkPipelineShaderStageCreateInfo &lastStage = m_shaderStages.back();

	lastStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	lastStage.pNext = NULL;
	lastStage.pSpecializationInfo = NULL;
	lastStage.flags = 0;
	lastStage.module = VK_NULL_HANDLE;
	lastStage.stage = stage; // VK_SHADER_STAGE_VERTEX_BIT or VK_SHADER_STAGE_FRAGMENT_BIT
	lastStage.pName = name;

	// use given spirv code as-is
	m_spirv.resize(size);
	memcpy(m_spirv.data(), spirv, size * sizeof(unsigned int));

	m_shaderInfo.codeSize = m_spirv.size() * sizeof(unsigned int);
	m_shaderInfo.pCode = m_spirv.data();

	return true;
}

bool vikaShaderModule::create()
{
	for (size_t i = 0; i < m_shaderStages.size(); i++)
	{
		m_res = vkCreateShaderModule(m_logDevice->getDevice(), &m_shaderInfo, NULL, &m_shaderStages[i].module);
		if (m_res != VK_SUCCESS)
		{
			return false;
		}
	}
	return true;
}

void vikaShaderModule::destroy()
{
	for (size_t i = 0; i < m_shaderStages.size(); i++)
	{
		vkDestroyShaderModule(m_logDevice->getDevice(), m_shaderStages[i].module, NULL);
	}
	m_shaderStages.clear();
}

