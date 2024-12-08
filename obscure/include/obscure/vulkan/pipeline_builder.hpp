#ifndef OBSCURE_VULKAN_PIPELINE_BUILDER_DEFINITION
#define OBSCURE_VULKAN_PIPELINE_BUILDER_DEFINITION 1
#include <array>
#include <optional>
#include <ranges>
#include "obscure/utils/value_list.hpp"
#include "glfw_vulkan_include.hpp"


namespace obscure
{
	namespace vulkan
	{

		template<uint32_t ShaderCount, uint32_t DynamicCount>
		struct pipeline_builder
		{
			std::array<vk::PipelineShaderStageCreateInfo, ShaderCount> shader_stages;

			std::optional<vk::PipelineVertexInputStateCreateInfo> input_state;

			std::optional<vk::PipelineInputAssemblyStateCreateInfo> assembly_state;

			std::optional<vk::PipelineTessellationStateCreateInfo> tesselation_state;

			std::optional<vk::PipelineViewportStateCreateInfo> viewport_state;

			std::optional<vk::PipelineRasterizationStateCreateInfo> rasterization_state;

			std::optional<vk::PipelineMultisampleStateCreateInfo> multisample_state;

			std::optional<vk::PipelineDepthStencilStateCreateInfo> depth_stensil_state;

			std::optional<vk::PipelineColorBlendStateCreateInfo> color_blend_state;

			std::array<vk::DynamicState, DynamicCount> dynamic_states;

			vk::PipelineLayout layout;

			vk::RenderPass render_pass;

			uint32_t sub_pass;

			vk::Pipeline base_pipeline;

			uint32_t base_index;

			template<typename T>
			const T* get_ptr(const std::optional<T> & opt)
			{
				if (opt.has_value())
				{
					return &opt.value();
				}
				else
				{
					return nullptr;
				}
			}

			vk::GraphicsPipelineCreateInfo get_create_info() const&
			{
				std::optional<vk::PipelineDynamicStateCreateInfo> dynamic_state;

				if constexpr (DynamicCount)
				{
					dynamic_state = vk::PipelineDynamicStateCreateInfo
					{
						{},
						dynamic_states
					};
				}

				return vk::GraphicsPipelineCreateInfo
				{
					{},
					shader_stages,
					get_ptr(input_state),
					get_ptr(assembly_state),
					get_ptr(tesselation_state),
					get_ptr(viewport_state),
					get_ptr(rasterization_state),
					get_ptr(multisample_state),
					get_ptr(depth_stensil_state),
					get_ptr(color_blend_state),
					get_ptr(dynamic_state),
					layout,
					render_pass,
					sub_pass,
					base_pipeline,
					base_index
				};
			}
		};

		template<typename TShaderStages>
		std::array<vk::PipelineShaderStageCreateInfo, TShaderStages::size()> get_shader_create_info(std::array<vk::ShaderModule, TShaderStages::size()> const& shaders) = delete;

		template<vk::ShaderStageFlagBits ... Flags>
		std::array<vk::PipelineShaderStageCreateInfo, value_list<vk::ShaderStageFlagBits, Flags ...>::size()> get_shader_create_info< value_list<vk::ShaderStageFlagBits, Flags ...>>(std::array<vk::ShaderModule, value_list<vk::ShaderStageFlagBits, Flags ...>::size()> const& shaders)
		{
			constexpr auto ShaderFlagBits = value_list<vk::ShaderStageFlagBits, Flags ...>::get_values();
			std::array<vk::PipelineShaderStageCreateInfo, value_list<vk::ShaderStageFlagBits, Flags ...>::size()> result{};
			for (size_t index = 0; index < value_list<vk::ShaderStageFlagBits, Flags ...>::size(); ++index)
			{
				result[index] = vk::PipelineShaderStageCreateInfo
				{
					{},
					ShaderFlagBits[index],
					shaders[index],
					"main"
				};
			}

			return result;
		}

		template<typename TShaderStages, typename TDynamicStates>
		pipeline_builder<TShaderStages::size(), TDynamicStates::size()> default_pipeline_builder(
			std::array<vk::ShaderModule, TShaderStages::size()> const& shaders
		)
		{
			pipeline_builder<TShaderStages::size(), TDynamicStates::size()> result{};
			result.shader_stages = get_shader_create_info<TShaderStages>(shaders);
		}
	}
}
#endif