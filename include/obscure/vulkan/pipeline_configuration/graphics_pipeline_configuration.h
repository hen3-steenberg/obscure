#ifndef OBSCURE_VULKAN_GRAPHICS_PIPELINE_CONFIGURATION
#define OBSCURE_VULKAN_GRAPHICS_PIPELINE_CONFIGURATION 1
#include <span>
#include <unordered_map>
#include <filesystem>
#include "obscure/vulkan/shader_module.h"
#include "obscure/vulkan/pipeline_layout.h"
#include "obscure/vulkan/render_pass.h"

namespace obscure
{
	namespace vulkan
	{

		struct graphics_pipeline_configuration
		{
			virtual pipeline_layout layout() = 0;

			virtual std::span<std::filesystem::path> shader_stage_paths() = 0;
			virtual std::span<VkPipelineShaderStageCreateInfo> shader_stages(std::unordered_map<std::filesystem::path, shader_module> const& loaded_shaders) = 0;
			virtual VkPipelineDynamicStateCreateInfo* dynamic_state() = 0;
			virtual VkPipelineVertexInputStateCreateInfo* vertex_input_state() = 0;
			virtual VkPipelineInputAssemblyStateCreateInfo* input_assembly_state() = 0;
			virtual VkPipelineViewportStateCreateInfo* viewport_state() = 0;
			virtual VkPipelineMultisampleStateCreateInfo* multisample_state() = 0;
			virtual VkPipelineRasterizationStateCreateInfo* rasterization_state() = 0;
			virtual VkPipelineColorBlendStateCreateInfo* color_blend_state() = 0;
			virtual VkPipelineDepthStencilStateCreateInfo* depth_stencil_state() = 0;
			
			VkGraphicsPipelineCreateInfo parse_configuration(render_pass pass, std::unordered_map<std::filesystem::path, shader_module> const& loaded_shaders);
		};
	}
}
#endif