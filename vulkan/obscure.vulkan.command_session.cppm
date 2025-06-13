module;
#include <functional>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.command_session;
export import obscure.vulkan.pipeline;
import obscure.properties;
export import obscure.vulkan.swap_chain;

export namespace obscure::vulkan
{
    template<pipeline_definition TPipeline>
    using draw_call_t = typename TPipeline::draw_calls;

    template<std::size_t index, pipeline_definition TPipeline>
    struct draw_calls_impl : draw_call_t<TPipeline>
    {
    private:
        [[nodiscard]] std::size_t get_pipeline_index() const final
        {
            return index;
        };
    };

    template<std::size_t index, pipeline_definition ... TPipelines>
    struct draw_call_collection;


    template<std::size_t index, pipeline_definition TPipeline, pipeline_definition ... TPipelines>
    struct draw_call_collection<index, TPipeline, TPipelines...> : draw_calls_impl<index, TPipeline>, draw_call_collection<index + 1, TPipelines...>
    {};

    template<size_t index>
    struct draw_call_collection<index>
    {};


    template<pipeline_definition ... TPipelines>
    struct command_session final : draw_call_collection<0, TPipelines...>
    {
    private:
        vk::CommandBuffer command_buffer;
        std::uint32_t frame_index;
        std::span<vk::Pipeline> pipeline_refs;
        std::span<vk::PipelineLayout> pipeline_layout_refs;
        vk::Extent2D extent;
        std::span<vk::DescriptorSetLayout> uniform_descriptor_set_layouts;
        std::span<vk::DescriptorSetLayout> texture_descriptor_set_layouts;



        [[nodiscard]] std::span<vk::Pipeline> get_all_pipelines() const final
        {
            return pipeline_refs;
        }

        [[nodiscard]] std::span<vk::DescriptorSetLayout> get_all_uniform_descriptor_layouts() const final
        {
            return uniform_descriptor_set_layouts;
        }

        [[nodiscard]] std::span<vk::PipelineLayout> get_all_layouts() const final
        {
            return pipeline_layout_refs;
        }

        [[nodiscard]] std::span<vk::DescriptorSetLayout> get_all_texture_descriptor_layouts() const final
        {
            return texture_descriptor_set_layouts;
        }
    public:

        command_session(vk::CommandBuffer _command_buffer, std::uint32_t index, swap_chain_ref swap_chain, pipeline_collection<sizeof...(TPipelines)> & pipeline_collection)
            : command_buffer(_command_buffer),
                frame_index(index),
                pipeline_refs(pipeline_collection.pipelines),
                pipeline_layout_refs(pipeline_collection.pipeline_layouts),
                extent(swap_chain.get().extent),
                uniform_descriptor_set_layouts(pipeline_collection.uniform_descriptor_layouts),
                texture_descriptor_set_layouts(pipeline_collection.texture_descriptor_layouts)
        {
            command_buffer.reset( vk::CommandBufferResetFlags {} );

            vk::CommandBufferBeginInfo begin_info {};

            command_buffer.begin(begin_info);

            vk::ClearValue clear_values[2] = { get_clear_color(), vk::ClearDepthStencilValue{1.0f, 0} };

            vk::RenderPassBeginInfo render_info {
                swap_chain.get().render_pass,
                swap_chain.get().framebuffers[frame_index],
                { {0,0}, extent },
                2,
                clear_values
            };

            command_buffer.beginRenderPass(render_info, vk::SubpassContents::eInline);
        }

        command_session(const command_session & other) = delete;
        command_session(command_session && other) noexcept
            : command_buffer(other.command_buffer),
              frame_index(other.frame_index),
              pipeline_refs(other.pipeline_refs),
              extent(other.extent)
        {
            other.command_buffer = VK_NULL_HANDLE;
        }

        [[nodiscard]] std::uint32_t get_frame_index() const final
        {
            return frame_index;
        }

        [[nodiscard]] vk::Extent2D get_extent() const final
        {
            return extent;
        }

        [[nodiscard]] vk::CommandBuffer get_command_buffer() const final
        {
            return command_buffer;
        }


        ~command_session()
        {
            if(command_buffer != VK_NULL_HANDLE)
            {
                command_buffer.endRenderPass();
                command_buffer.end();
            }
        }
    };
}