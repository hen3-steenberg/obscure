#ifndef OBSCURE_VULKAN_BUILTIN_PIPELINE_TEST_TRIANGLE_DEFINITION
#define OBSCURE_VULKAN_BUILTIN_PIPELINE_TEST_TRIANGLE_DEFINITION 1
#include "obscure/utils/key_set.hpp"
#include "obscure/vulkan/builtin_shaders.hpp"
#include "obscure/vulkan/pipeline_definition.hpp"
#include <array>

namespace obscure
{
	namespace vulkan
	{
		namespace builtin_pipelines
		{
			struct test_triangle
			{
				using shader_list = obscure::make_set<obscure::vulkan::builtin_shader::test_triangle_fragment, obscure::vulkan::builtin_shader::test_triangle_vertex>;

				static static_pipeline_builder<2, 2, 0, 0> initialize(vk::Device device, vk::RenderPass render_pass, std::array<vk::ShaderModule, 2> const& shaders)
				{
#pragma region shaders_fixed_functions
					static_pipeline_builder<2, 2, 0, 0> result = default_pipeline_builder<0, 0, vk::PrimitiveTopology::eTriangleList, vk::PolygonMode::eFill,
					vk::FrontFace::eClockwise,vk::ShaderStageFlagBits::eFragment, vk::ShaderStageFlagBits::eVertex>(render_pass, shaders, {}, {});
#pragma endregion

#pragma region pipeline_layout
					vk::PipelineLayoutCreateInfo pipeline_info {
						{},
						0,
						nullptr,
						0,
						nullptr
					};

					result.layout = device.createPipelineLayout(pipeline_info);
#pragma endregion
					return result;
				}

				struct draw_calls : draw_call_base {
					void test_triangle() const
					{
						bind_pipeline();

						vk::Viewport viewport {
							0.0f,
							0.0f,
							static_cast<float>(get_extent().width),
							static_cast<float>(get_extent().height),
							0.0f,
							1.0f
						};

						get_command_buffer().setViewport(0, 1, &viewport);

						vk::Rect2D scissor {
							{0, 0},
							get_extent()
						};

						get_command_buffer().setScissor(0, 1, &scissor);

						get_command_buffer().draw(3, 1, 0, 0);
					}
				};
			};

			static_assert(pipeline_definition<test_triangle>);

		}
	}
}
#endif