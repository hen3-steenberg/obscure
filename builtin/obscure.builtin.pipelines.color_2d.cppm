module;
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>
export module obscure.builtin.pipelines.color_2d;

export import obscure.builtin.shaders;
export import obscure.vulkan.pipeline;
export import obscure.vulkan.buffer;

export namespace obscure::builtin::pipeline
{

    struct color_2d_vertex {
        glm::vec2 position;
        glm::vec3 color;
    };

    // struct color_2d_uniform
    // {
    //     glm::mat4 model;
    //     glm::mat4 view;
    //     glm::mat4 proj;
    // };

    struct color_2d {
        using shader_list =
            obscure::make_set<obscure::builtin::shader::color_2d_vertex, obscure::builtin::shader::color_fragment>;

        static obscure::vulkan::static_pipeline_builder<2, 2, 1, 2>
        initialize(vk::Device device,
                   vk::RenderPass render_pass,
                   vk::SampleCountFlagBits samples,
                   std::array<vk::ShaderModule, 2> const& shaders)
        {
#pragma region shaders_fixed_functions
            auto result = obscure::vulkan::default_pipeline_builder<1,
                                                                    2,
                                                                    vk::PrimitiveTopology::eTriangleList,
                                                                    vk::PolygonMode::eFill,
                                                                    vk::FrontFace::eCounterClockwise,
                                                                    vk::ShaderStageFlagBits::eVertex,
                                                                    vk::ShaderStageFlagBits::eFragment>(
                render_pass,
                samples,
                shaders,
                { vk::VertexInputBindingDescription{ 0, sizeof(color_2d_vertex), vk::VertexInputRate::eVertex } },
                { vk::VertexInputAttributeDescription{
                      0, 0, vk::Format::eR32G32Sfloat, offsetof(color_2d_vertex, position) },
                  vk::VertexInputAttributeDescription{
                      1, 0, vk::Format::eR32G32B32Sfloat, offsetof(color_2d_vertex, color) } });

#pragma endregion

#pragma region uniforms

            // auto uniforms = obscure::vulkan::create_uniform_descriptor_bindings<1>({ vk::ShaderStageFlagBits::eVertex
            // }); vk::DescriptorSetLayoutCreateInfo uniform_set_layout_info{
            //     {},
            //     uniforms
            // };
            //
            // result.uniform_set_layout = device.createDescriptorSetLayout(uniform_set_layout_info);

#pragma endregion

#pragma region push_constants
            vk::PushConstantRange push_constants{ vk::ShaderStageFlagBits::eVertex, 0, sizeof(glm::mat4) };
#pragma endregion

#pragma region pipeline_layout
            vk::PipelineLayoutCreateInfo pipeline_info{ {},
                                                        0,       // 1,
                                                        nullptr, //&result.uniform_set_layout,
                                                        1,
                                                        &push_constants };

            result.layout = device.createPipelineLayout(pipeline_info);
#pragma endregion
            return result;
        }

        struct draw_calls : obscure::vulkan::draw_call_base {

            using triangles_t = obscure::vulkan::vertex_buffer<color_2d_vertex>;

            template<vulkan::vk_index T>
            using index_buffer_t = obscure::vulkan::index_buffer<T>;

            void
            draw_color_2d(glm::mat4 world, glm::mat4 model, triangles_t const& triangles) const
            {
                bind_pipeline();

                vk::Viewport viewport{
                    0.0f, 0.0f, static_cast<float>(get_extent().width), static_cast<float>(get_extent().height),
                    0.0f, 1.0f
                };

                get_command_buffer().setViewport(0, 1, &viewport);

                vk::Rect2D scissor{ { 0, 0 }, get_extent() };

                get_command_buffer().setScissor(0, 1, &scissor);

                vk::Buffer buffers[] = { triangles.get_buffer() };
                vk::DeviceSize offsets[] = { 0 };

                get_command_buffer().bindVertexBuffers(0, 1, buffers, offsets);

                glm::mat4 transform = world * model;

                get_command_buffer().pushConstants(
                    get_pipeline_layout(), vk::ShaderStageFlagBits::eVertex, 0, sizeof(glm::mat4), &transform);

                get_command_buffer().draw(triangles.count(), 1, 0, 0);
            }

            template<vulkan::vk_index T>
            void
            draw_color_2d(glm::mat4 world,
                          glm::mat4 model,
                          triangles_t const& triangles,
                          index_buffer_t<T> const& indices)
            {
                bind_pipeline();

                vk::Viewport viewport{
                    0.0f, 0.0f, static_cast<float>(get_extent().width), static_cast<float>(get_extent().height),
                    0.0f, 1.0f
                };

                get_command_buffer().setViewport(0, 1, &viewport);

                vk::Rect2D scissor{ { 0, 0 }, get_extent() };

                get_command_buffer().setScissor(0, 1, &scissor);

                vk::Buffer buffers[] = { triangles.get_buffer() };
                vk::DeviceSize offsets[] = { 0 };

                get_command_buffer().bindVertexBuffers(0, 1, buffers, offsets);

                get_command_buffer().bindIndexBuffer(indices.get_buffer(), 0, index_buffer_t<T>::get_index_type());

                glm::mat4 transform = world * model;

                get_command_buffer().pushConstants(
                    get_pipeline_layout(), vk::ShaderStageFlagBits::eVertex, 0, sizeof(glm::mat4), &transform);

                get_command_buffer().drawIndexed(indices.count(), 1, 0, 0, 0);
            }
        };
    };

    static_assert(obscure::vulkan::pipeline_definition<color_2d>);
} // namespace obscure::builtin::pipeline