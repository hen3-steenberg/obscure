#ifndef OBSCURE_VULKAN_COMMAND_SESSION_DEFINITION
#define OBSCURE_VULKAN_COMMAND_SESSION_DEFINITION 1
#include "obscure/vulkan/pipeline_definition.hpp"
#include "obscure/vulkan/swap_chain.hpp"
#include "obscure/obscure_properties.hpp"

namespace obscure
{
    namespace vulkan
    {
        template<pipeline_definition TPipeline>
        using draw_call_t = typename TPipeline::draw_calls;


        template<std::size_t index, pipeline_definition ... TPipelines>
        struct draw_call_collection;

        template<std::size_t index, pipeline_definition TPipeline, pipeline_definition ... TPipelines>
        struct draw_calls_impl : draw_call_t<TPipeline>, draw_call_collection<index + 1, TPipelines...>::type
        {
            private:
                [[nodiscard]] std::size_t get_pipeline_index() const override
                {
                    return index;
                };
        };

        template<std::size_t index, pipeline_definition TPipeline, pipeline_definition ... TPipelines>
        struct draw_call_collection<index, TPipeline, TPipelines...>
        {
            using type = draw_calls_impl<index,TPipeline, TPipelines...>;
        };

        template<size_t index>
        struct draw_call_collection<index>
        {
            struct type {};
        };


        template<pipeline_definition ... TPipelines>
        struct command_session final : shared_draw_call_base, draw_call_collection<0, TPipelines...>::type
        {
            private:
                vk::CommandBuffer command_buffer;
                std::uint32_t frame_index;
                std::span<vk::Pipeline> pipeline_refs;
                vk::Extent2D extent;

                [[nodiscard]] vk::CommandBuffer get_command_buffer() const override
                {
                    return command_buffer;
                }

                [[nodiscard]] std::uint32_t get_frame_index() const override
                {
                    return frame_index;
                }

                [[nodiscard]] std::span<vk::Pipeline> get_all_pipelines() const override
                {
                    return pipeline_refs;
                }

                [[nodiscard]] vk::Extent2D get_extent() const override
                {
                    return extent;
                }
            public:

                  command_session(vk::CommandBuffer _command_buffer, std::uint32_t index, std::span<vk::Pipeline> pipeline_collection, swap_chain_ref swap_chain)
                      : command_buffer(_command_buffer), frame_index(index), pipeline_refs(pipeline_collection), extent(swap_chain.get().extent)
                  {
                      //command_buffer.reset( vk::CommandBufferResetFlags {} );

                      vk::CommandBufferBeginInfo begin_info {};

                      command_buffer.begin(begin_info);

                      vk::ClearValue clear_color { get_clear_color() };

                      vk::RenderPassBeginInfo render_info {
                          swap_chain.get().render_pass,
                          swap_chain.get().framebuffers[frame_index],
                          { {0,0}, extent },
                          1,
                          &clear_color
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
}
#endif