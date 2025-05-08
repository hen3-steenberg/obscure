module;
#include <span>
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
export module pipeline:draw_call_base;

export namespace obscure::vulkan
{
        struct shared_draw_call_base
        {
        protected:
            [[nodiscard]] virtual vk::CommandBuffer get_command_buffer() const = 0;
            [[nodiscard]] virtual std::uint32_t get_frame_index() const = 0;
            [[nodiscard]] virtual std::span<vk::Pipeline> get_all_pipelines() const = 0;
            [[nodiscard]] virtual vk::Extent2D get_extent() const = 0;

        public:
            virtual ~shared_draw_call_base() = default;

        };

        struct draw_call_base : virtual shared_draw_call_base
        {
        protected:
            [[nodiscard]] virtual std::size_t get_pipeline_index() const = 0;
            [[nodiscard]] vk::Pipeline get_graphics_pipeline() const
            {
                return get_all_pipelines()[get_pipeline_index()];
            }

            void bind_pipeline() const
            {
                get_command_buffer().bindPipeline(vk::PipelineBindPoint::eGraphics, get_graphics_pipeline());
            }
        public:
            ~draw_call_base() override = default;
        };
}