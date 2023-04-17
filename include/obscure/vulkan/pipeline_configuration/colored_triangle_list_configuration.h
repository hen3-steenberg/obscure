#ifndef OBSCURE_VULKAN_COLORED_TRIANGLE_LIST_CONFIGURATION_DEFINITION
#define OBSCURE_VULKAN_COLORED_TRIANGLE_LIST_CONFIGURATION_DEFINITION 1

#include <array>
#include "obscure/vulkan/pipeline_configuration/graphics_pipeline_configuration.h"
#include "obscure/vulkan/device.h"
#include "obscure/vulkan/swap_chain.h"

namespace obscure
{
	namespace vulkan
	{

		struct colored_triangle_list_configuration : obscure::vulkan::graphics_pipeline_configuration
		{

			std::array<VkPipelineShaderStageCreateInfo, 2> shaders;

			std::array<VkVertexInputBindingDescription, 1> input_bindings;

			std::array<VkVertexInputAttributeDescription, 2> input_attributes;

			VkPipelineDynamicStateCreateInfo _dynamic_state_info{};

			VkPipelineVertexInputStateCreateInfo _vertex_input_state{};

			VkPipelineInputAssemblyStateCreateInfo _input_assembly{};

			VkPipelineColorBlendAttachmentState color_blend_attatchment{};
			VkPipelineColorBlendStateCreateInfo _color_blend_state{};

			VkViewport viewport{};
			VkRect2D scissor{};
			VkPipelineViewportStateCreateInfo _viewport_state{};

			VkPipelineMultisampleStateCreateInfo _multisample_state{};

			VkPipelineRasterizationStateCreateInfo _rasterization_state{};

			obscure::vulkan::pipeline_layout _layout;


			colored_triangle_list_configuration(obscure::vulkan::device _device, obscure::vulkan::swap_chain const& _swap_chain, VkAllocationCallbacks const* _allocator);

			obscure::vulkan::pipeline_layout layout();

			std::span<std::filesystem::path> shader_stage_paths();
			std::span<VkPipelineShaderStageCreateInfo> shader_stages(std::unordered_map<std::filesystem::path, obscure::vulkan::shader_module> const& loaded_shaders);
			VkPipelineDynamicStateCreateInfo* dynamic_state();
			VkPipelineVertexInputStateCreateInfo* vertex_input_state();
			VkPipelineInputAssemblyStateCreateInfo* input_assembly_state();
			VkPipelineViewportStateCreateInfo* viewport_state();
			VkPipelineMultisampleStateCreateInfo* multisample_state();
			VkPipelineRasterizationStateCreateInfo* rasterization_state();
			VkPipelineColorBlendStateCreateInfo* color_blend_state();
			VkPipelineDepthStencilStateCreateInfo* depth_stencil_state();
		};
	}
}

#endif