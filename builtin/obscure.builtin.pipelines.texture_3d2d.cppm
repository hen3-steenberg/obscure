module;
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>
export module obscure.builtin.pipelines.texture_3d2d;

export import obscure.builtin.shaders;
export import obscure.vulkan.pipeline;
export import obscure.vulkan.buffer;
export import obscure.vulkan.texture;

export namespace obscure::builtin::pipeline
{

    struct texture_3d2d_vertex {
        glm::vec3 position;
        glm::vec2 texture_coord;
    };

    struct texture_3d2d {
        using shader_list = obscure::make_set<obscure::builtin::shader::texture_2d_vertex,
                                              obscure::builtin::shader::texture_2d_fragment>;

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
                { vk::VertexInputBindingDescription{ 0, sizeof(texture_3d2d_vertex), vk::VertexInputRate::eVertex } },
                { vk::VertexInputAttributeDescription{
                      0, 0, vk::Format::eR32G32B32Sfloat, offsetof(texture_3d2d_vertex, position) },
                  vk::VertexInputAttributeDescription{
                      1, 0, vk::Format::eR32G32Sfloat, offsetof(texture_3d2d_vertex, texture_coord) } });

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

            vk::DescriptorSetLayoutBinding texture_binding{
                0, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment, nullptr
            };

            vk::DescriptorSetLayoutCreateInfo texture_set_layout_info{ {}, 1, &texture_binding };

            result.texture_set_layout = device.createDescriptorSetLayout(texture_set_layout_info);
#pragma region textures
#pragma endregion

#pragma region push_constants
            vk::PushConstantRange push_constants{ vk::ShaderStageFlagBits::eVertex, 0, sizeof(glm::mat4) };
#pragma endregion

#pragma region pipeline_layout
            vk::PipelineLayoutCreateInfo pipeline_info{ {}, 1, &result.texture_set_layout, 1, &push_constants };

            result.layout = device.createPipelineLayout(pipeline_info);
#pragma endregion
            return result;
        }

        struct draw_calls : obscure::vulkan::draw_call_base {

            using triangles_t = obscure::vulkan::vertex_buffer<texture_3d2d_vertex>;
            template<vk::SamplerAddressMode modeU, vk::SamplerAddressMode modeV, vk::SamplerAddressMode modeW>
            using texture_t = obscure::vulkan::rgba_2d_texture<modeU, modeV, modeW>;

            template<vulkan::vk_index T>
            using index_buffer_t = obscure::vulkan::index_buffer<T>;

            template<vk::SamplerAddressMode modeU, vk::SamplerAddressMode modeV, vk::SamplerAddressMode modeW>
            void
            draw_texture_2d(glm::mat4 world,
                            glm::mat4 model,
                            triangles_t const& triangles,
                            texture_t<modeU, modeV, modeW> const& texture) const
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

                get_command_buffer().bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                                        get_pipeline_layout(),
                                                        0,
                                                        1,
                                                        texture.descriptor_sets[get_frame_index()],
                                                        0,
                                                        nullptr);

                get_command_buffer().pushConstants(
                    get_pipeline_layout(), vk::ShaderStageFlagBits::eVertex, 0, sizeof(glm::mat4), &transform);

                get_command_buffer().draw(triangles.count(), 1, 0, 0);
            }

            template<vulkan::vk_index T,
                     vk::SamplerAddressMode modeU,
                     vk::SamplerAddressMode modeV,
                     vk::SamplerAddressMode modeW>
            void
            draw_texture_2d(glm::mat4 world,
                            glm::mat4 model,
                            triangles_t const& triangles,
                            index_buffer_t<T> const& indices,
                            texture_t<modeU, modeV, modeW> const& texture)
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

                get_command_buffer().bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                                        get_pipeline_layout(),
                                                        0,
                                                        1,
                                                        &texture.descriptor_sets[get_frame_index()],
                                                        0,
                                                        nullptr);

                get_command_buffer().pushConstants(
                    get_pipeline_layout(), vk::ShaderStageFlagBits::eVertex, 0, sizeof(glm::mat4), &transform);

                get_command_buffer().drawIndexed(indices.count(), 1, 0, 0, 0);
            }
        };
    };

    static_assert(obscure::vulkan::pipeline_definition<texture_3d2d>);
} // namespace obscure::builtin::pipeline