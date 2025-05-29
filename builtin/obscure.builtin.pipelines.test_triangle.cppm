module;
#include <vulkan/vulkan.hpp>
export module obscure.builtin.pipelines.test_triangle;
export import obscure.builtin.shaders;
export import obscure.vulkan.pipeline;

export namespace obscure::builtin::pipeline
{
    struct test_triangle
    {
        using shader_list = obscure::make_set<
            obscure::builtin::shader::test_triangle_vertex,
            obscure::builtin::shader::color_fragment
        >;

        static obscure::vulkan::static_pipeline_builder<2, 2, 0, 0> initialize(
            vk::Device device,
            vk::RenderPass render_pass,
            std::array<vk::ShaderModule, 2> const& shaders)
        {
#pragma region shaders_fixed_functions
            auto result = obscure::vulkan::default_pipeline_builder<0, 0,
                vk::PrimitiveTopology::eTriangleList,
                vk::PolygonMode::eFill,
                vk::FrontFace::eClockwise,
                vk::ShaderStageFlagBits::eVertex,
                vk::ShaderStageFlagBits::eFragment
            >(render_pass, shaders, {}, {});

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

        struct draw_calls : obscure::vulkan::draw_call_base {
            void draw_test_triangle() const
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

    static_assert(obscure::vulkan::pipeline_definition<test_triangle>);
}