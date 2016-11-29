// vikaShaderModule : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASHADERMODULE_H_
#define _VIKASHADERMODULE_H_

#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;

class vikaShaderModule
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;

	// compiled shader code (spir-v)
	std::vector<unsigned int> m_spirv;

public:
	//std::string m_stageName;
	std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
	//VkPipelineShaderStageCreateInfo m_shaderStage = {};
	VkShaderModuleCreateInfo m_shaderInfo = {};

public:
	vikaShaderModule(vikaDevice *logDevice);
	virtual ~vikaShaderModule();

	// VK_SHADER_STAGE_VERTEX_BIT or VK_SHADER_STAGE_FRAGMENT_BIT, "main", glsl code
	//bool addStage(VkShaderStageFlagBits stage, const char *name, const char *shaderText);
	bool addStage(VkShaderStageFlagBits stage, const char *name, unsigned int *spirv, size_t size);

	bool create();
	void destroy();
};

#endif
