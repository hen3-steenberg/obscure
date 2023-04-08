#include "obscure/vulkan/pipeline_configuration/graphics_pipeline_configuration.h"

VkGraphicsPipelineCreateInfo obscure::vulkan::graphics_pipeline_configuration::parse_configuration(render_pass pass, std::unordered_map<std::filesystem::path, shader_module> const& loaded_shaders)
{
	VkGraphicsPipelineCreateInfo result{};
	result.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	result.pNext = nullptr;

	std::span<VkPipelineShaderStageCreateInfo> modules = this->shader_stages(loaded_shaders);

	result.stageCount = static_cast<uint32_t>(modules.size());
	result.pStages = modules.data();

	result.pVertexInputState = this->vertex_input_state();
	result.pInputAssemblyState = this->input_assembly_state();
	result.pViewportState = this->viewport_state();
	result.pRasterizationState = this->rasterization_state();
	result.pMultisampleState = this->multisample_state();
	result.pDepthStencilState = this->depth_stencil_state();
	result.pColorBlendState = this->color_blend_state();
	result.pDynamicState = this->dynamic_state();
	 
	result.layout = layout().get_handle();

	result.renderPass = pass.get_handle();
	result.subpass = 0;

	result.basePipelineHandle = VK_NULL_HANDLE; // Optional
	result.basePipelineIndex = -1; // Optional

	return result;
}