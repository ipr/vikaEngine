// vikaSubpass : 
//
// Ilkka Prusi 2016 <ilkka.prusi@gmail.com>

#include "stdafx.h"
#include "vikaSubpass.h"
#include "vikaDevice.h"
#include "vikaPhysDevice.h"

#include <vulkan/vulkan.h>

vikaSubpass::vikaSubpass(vikaDevice *logDevice, vikaPhysDevice *physDevice) :
	m_res(VK_SUCCESS),
	m_logDevice(logDevice),
	m_physDevice(physDevice)
{
}

vikaSubpass::~vikaSubpass()
{
	destroy();
}

bool vikaSubpass::create()
{
	m_subpasses.resize(2);

    m_subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    m_subpasses[0].flags = 0;
    m_subpasses[0].inputAttachmentCount = 0;
    m_subpasses[0].pInputAttachments = NULL;
    m_subpasses[0].colorAttachmentCount = 0;
    m_subpasses[0].pColorAttachments = NULL;
    m_subpasses[0].pResolveAttachments = NULL;
    //m_subpasses[0].pDepthStencilAttachment = &depth_reference;
    m_subpasses[0].preserveAttachmentCount = 0;
    m_subpasses[0].pPreserveAttachments = NULL;

    m_subpasses[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    m_subpasses[1].flags = 0;
    m_subpasses[1].inputAttachmentCount = 0;
    m_subpasses[1].pInputAttachments = NULL;
    m_subpasses[1].colorAttachmentCount = 1;
    //m_subpasses[1].pColorAttachments = &color_reference;
    m_subpasses[1].pResolveAttachments = NULL;
    //m_subpasses[1].pDepthStencilAttachment = &depth_reference;
    m_subpasses[1].preserveAttachmentCount = 0;
    m_subpasses[1].pPreserveAttachments = NULL;

    m_dependency.srcSubpass = 0;
    m_dependency.dstSubpass = 1;
    m_dependency.dependencyFlags = 0;
    m_dependency.srcStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
    m_dependency.dstStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
    m_dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
                               VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    m_dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
                               VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

	return true;
}

void vikaSubpass::destroy()
{
}

