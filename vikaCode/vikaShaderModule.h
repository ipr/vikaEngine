// vikaShaderModule : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#ifndef _VIKASHADERMODULE_H_
#define _VIKASHADERMODULE_H_

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class vikaDevice;

class vikaShaderModule
{
protected:
	VkResult m_res;
	vikaDevice *m_logDevice;

	VkShaderModuleCreateInfo m_shaderInfo = {};
	VkShaderModule m_shader;

public:
	vikaShaderModule(vikaDevice *logDevice);
	virtual ~vikaShaderModule();

	bool create();
	void destroy();
};

#endif
