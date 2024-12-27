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
		template<typename T>
		concept pipeline_builder = requires(T t)
		{
			{ t.get_create_info() } -> std::convertible_to<vk::GraphicsPipelineCreateInfo>;
			{ t.get_layout() } -> std::convertible_to<vk::PipelineLayout>;
		};

		template<uint32_t ShaderCount, uint32_t DynamicCount, uint32_t VertexBindingCount, uint32_t VertexAttributeCount>
		struct static_pipeline_builder
		{
			std::array<vk::PipelineShaderStageCreateInfo, ShaderCount> shader_stages;

			vk::PipelineVertexInputStateCreateInfo vertex_input_state;

			std::array<vk::VertexInputBindingDescription, VertexBindingCount> vertex_bindings;

			std::array<vk::VertexInputAttributeDescription, VertexAttributeCount> vertex_attributes;

			std::optional<vk::PipelineInputAssemblyStateCreateInfo> assembly_state;

			std::optional<vk::PipelineTessellationStateCreateInfo> tesselation_state;

			std::optional<vk::PipelineViewportStateCreateInfo> viewport_state;

			std::optional<vk::PipelineRasterizationStateCreateInfo> rasterization_state;

			std::optional<vk::PipelineMultisampleStateCreateInfo> multisample_state;

			std::optional<vk::PipelineDepthStencilStateCreateInfo> depth_stensil_state;

			vk::PipelineColorBlendAttachmentState color_blend_attachment;

			vk::PipelineColorBlendStateCreateInfo color_blend_state;

			std::array<vk::DynamicState, DynamicCount> dynamic_states;

			std::optional<vk::PipelineDynamicStateCreateInfo> dynamic_state;

			vk::PipelineLayout layout;

			vk::RenderPass render_pass;

			uint32_t sub_pass {0};

			vk::Pipeline base_pipeline {nullptr};

			int32_t base_index {-1};

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

			[[nodiscard]] vk::PipelineLayout get_layout() const {
				return layout;
			}

			[[nodiscard]] vk::GraphicsPipelineCreateInfo get_create_info()
			{
				if constexpr (DynamicCount)
				{
					dynamic_state = vk::PipelineDynamicStateCreateInfo
					{
						{},
						dynamic_states
					};
				}

				vertex_input_state = vk::PipelineVertexInputStateCreateInfo {
					{},
					vertex_bindings,
					vertex_attributes
				};

				color_blend_state = vk::PipelineColorBlendStateCreateInfo {
					{},
					vk::False,
					vk::LogicOp::eCopy,
					1U,
					&color_blend_attachment,
	{0.0f, 0.0f, 0.0f, 0.0f}
				};

				return vk::GraphicsPipelineCreateInfo
				{
					{},
					shader_stages,
					&vertex_input_state,
					get_ptr(assembly_state),
					get_ptr(tesselation_state),
					get_ptr(viewport_state),
					get_ptr(rasterization_state),
					get_ptr(multisample_state),
					get_ptr(depth_stensil_state),
					&color_blend_state,
					get_ptr(dynamic_state),
					layout,
					render_pass,
					sub_pass,
					base_pipeline,
					base_index
				};
			}
		};

		static_assert(pipeline_builder<static_pipeline_builder<2,2,2,2>>);

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

		template<
			uint32_t VertexBindingCount,
			uint32_t VertexAttributeCount,
			vk::PrimitiveTopology topology,
			vk::PolygonMode polygonMode,
			vk::FrontFace frontFace,
			vk::ShaderStageFlagBits ... Flags>
		static_pipeline_builder<sizeof...(Flags), 2, VertexBindingCount, VertexAttributeCount> default_pipeline_builder(
			vk::RenderPass render_pass,
			std::array<vk::ShaderModule, sizeof...(Flags)> const& shaders,
			std::array<vk::VertexInputBindingDescription, VertexBindingCount> vertex_bindings,
			std::array<vk::VertexInputAttributeDescription, VertexAttributeCount> vertex_attributes
		)
		{
			static_pipeline_builder<sizeof...(Flags), 2, VertexBindingCount, VertexAttributeCount> result{};
#pragma region Render Pass
			result.render_pass = render_pass;
#pragma endregion

#pragma region ShaderStage
			using TShaderStages = value_list<vk::ShaderStageFlagBits, Flags...>;

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

#pragma region ViewportState
			result.viewport_state = vk::PipelineViewportStateCreateInfo {
				{},
				1,
				nullptr,	//dynamic state
				1,
				nullptr		//dynamic state
			};
#pragma endregion

#pragma region RasterizationState
			result.rasterization_state = vk::PipelineRasterizationStateCreateInfo {
				{},
				vk::False,
				vk::False,
				polygonMode,
				vk::CullModeFlagBits::eBack,
				frontFace,
				vk::False,
				0.0f,
				0.0f,
				0.0f,
				1.0f
			};
#pragma endregion

#pragma region MultisampleState
			result.multisample_state = vk::PipelineMultisampleStateCreateInfo {
				{},
				vk::SampleCountFlagBits::e1,
				vk::False,
				1.0f,
				nullptr,
				vk::False,
				vk::False
			};
#pragma endregion

#pragma region ColorBlendState
			result.color_blend_attachment = vk::PipelineColorBlendAttachmentState {
				vk::True,
				vk::BlendFactor::eSrcAlpha,
				vk::BlendFactor::eOneMinusSrcAlpha,
				vk::BlendOp::eAdd,
				vk::BlendFactor::eOne,
				vk::BlendFactor::eZero,
				vk::BlendOp::eAdd
			};
#pragma endregion

#pragma region DynamicState
			result.dynamic_states = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
#pragma endregion

			return result;
		}
	}
}
#endif