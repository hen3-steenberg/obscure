module;
#include <span>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.pipeline:draw_call_base;

export namespace obscure::vulkan
{
    struct shared_draw_call_base {
    protected:
        [[nodiscard]] virtual vk::CommandBuffer
        get_command_buffer() const = 0;
        [[nodiscard]] virtual std::uint32_t
        get_frame_index() const = 0;
        [[nodiscard]] virtual std::span<vk::Pipeline>
        get_all_pipelines() const = 0;
        [[nodiscard]] virtual std::span<vk::PipelineLayout>
        get_all_layouts() const = 0;
        [[nodiscard]] virtual vk::Extent2D
        get_extent() const = 0;
        [[nodiscard]] virtual std::span<vk::DescriptorSetLayout>
        get_all_uniform_descriptor_layouts() const = 0;
        [[nodiscard]] virtual std::span<vk::DescriptorSetLayout>
        get_all_texture_descriptor_layouts() const = 0;

    public:
        virtual ~shared_draw_call_base() = default;
    };

    struct draw_call_base : virtual shared_draw_call_base {
    protected:
        [[nodiscard]] virtual std::size_t
        get_pipeline_index() const = 0;
        [[nodiscard]] vk::Pipeline
        get_graphics_pipeline() const
        {
            return get_all_pipelines()[get_pipeline_index()];
        }

        [[nodiscard]] vk::PipelineLayout
        get_pipeline_layout() const
        {
            return get_all_layouts()[get_pipeline_index()];
        }

        [[nodiscard]] vk::DescriptorSetLayout
        get_uniform_layout() const
        {
            return get_all_uniform_descriptor_layouts()[get_pipeline_index()];
        }

        [[nodiscard]] vk::DescriptorSetLayout
        get_texture_layout() const
        {
            return get_all_texture_descriptor_layouts()[get_pipeline_index()];
        }

        void
        bind_pipeline() const
        {
            get_command_buffer().bindPipeline(vk::PipelineBindPoint::eGraphics, get_graphics_pipeline());
        }

    public:
        ~draw_call_base() override = default;
    };
} // namespace obscure::vulkan