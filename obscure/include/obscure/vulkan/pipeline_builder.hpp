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

		template<uint32_t ShaderCount, uint32_t DynamicCount, uint32_t VertexBindingCount, uint32_t VertexAttributeCount>
		struct pipeline_builder
		{
			std::array<vk::PipelineShaderStageCreateInfo, ShaderCount> shader_stages;

			std::array<vk::VertexInputBindingDescription, VertexBindingCount> vertex_bindings;

			std::array<vk::VertexInputAttributeDescription, VertexAttributeCount> vertex_attributes;

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
			static constexpr const T* get_ptr(const std::optional<T> & opt)
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
				std::optional<vk::PipelineDynamicStateCreateInfo> dynamic_state{};

				if constexpr (DynamicCount)
				{
					dynamic_state = vk::PipelineDynamicStateCreateInfo
					{
						{},
						dynamic_states
					};
				}

				vk::PipelineVertexInputStateCreateInfo vertex_input_state {
					{},
					vertex_bindings,
					vertex_attributes
				};

				return vk::GraphicsPipelineCreateInfo
				{
					{},
					shader_stages,
					&vertex_input_state
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

		template<typename TShaderStages, size_t size>
		std::array<vk::PipelineShaderStageCreateInfo, size> get_shader_create_info(std::array<vk::ShaderModule, size> const& shaders)
		{
			constexpr auto ShaderFlagBits = TShaderStages::get_values();
			std::array<vk::PipelineShaderStageCreateInfo, size> result{};
			for (size_t index = 0; index < size; ++index)
			{
				result[index] = vk::PipelineShaderStageCreateInfo {
					{},
					ShaderFlagBits[index],
					shaders[index],
					"main"
				};
			}

			return result;
		}

		template<uint32_t VertexBindingCount, uint32_t VertexAttributeCount, vk::PrimitiveTopology topology, vk::ShaderStageFlagBits ... Flags>
		pipeline_builder<sizeof...(Flags), 2, VertexBindingCount, VertexAttributeCount> default_pipeline_builder(
			std::array<vk::ShaderModule, sizeof...(Flags)> const& shaders,
			std::array<vk::VertexInputBindingDescription, VertexBindingCount> vertex_bindings,
			std::array<vk::VertexInputAttributeDescription, VertexAttributeCount> vertex_attributes
		)
		{
#pragma region ShaderStage
			using TShaderStages = value_list<vk::ShaderStageFlagBits, Flags...>;
			pipeline_builder<TShaderStages::size(), 2, VertexBindingCount, VertexAttributeCount> result{};
			result.shader_stages = get_shader_create_info<TShaderStages, TShaderStages::size()>(shaders);
#pragma endregion

#pragma region VertexInputState
			result.vertex_bindings = vertex_bindings;
			result.vertex_attributes = vertex_attributes;
#pragma endregion

#pragma region InputAssemblyState
			constexpr vk::Bool32 EnableRestart = 
				(topology == vk::PrimitiveTopology::eLineStrip) ||
				(topology == vk::PrimitiveTopology::eTriangleStrip) ||
				(topology == vk::PrimitiveTopology::eLineStripWithAdjacency) ||
				(topology == vk::PrimitiveTopology::eTriangleStripWithAdjacency) ?
				vk::True : vk::False;

			result.assembly_state = vk::PipelineInputAssemblyStateCreateInfo {
				{},
				topology,
				EnableRestart
			};
#pragma endregion

#pragma region DynamicState
			result.dynamic_states = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
#pragma endregion
		}
	}
}
#endif