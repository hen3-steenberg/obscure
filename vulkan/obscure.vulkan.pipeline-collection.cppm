module;
#include <array>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.pipeline:collection;
import :builder;
import :definition;
import obscure.vulkan.result;

export namespace obscure::vulkan
{
    template<std::size_t Size>
    struct pipeline_collection : private vk::Device {
        std::array<vk::Pipeline, Size> pipelines;
        std::array<vk::PipelineLayout, Size> pipeline_layouts;
        std::array<vk::DescriptorSetLayout, Size> uniform_descriptor_layouts;
        std::array<vk::DescriptorSetLayout, Size> texture_descriptor_layouts;

    private:
        [[nodiscard]] vk::Device
        get_device() const noexcept
        {
            return static_cast<vk::Device>(*this);
        }

        template<pipeline_builder... PipelineBuilders>
            requires(sizeof...(PipelineBuilders) == Size)
        explicit pipeline_collection(vk::Device device, PipelineBuilders... builders)
            : vk::Device(device)
            , pipeline_layouts{ static_cast<vk::PipelineLayout>(builders.get_layout())... }
            , uniform_descriptor_layouts{ static_cast<vk::DescriptorSetLayout>(
                  builders.get_uniform_descriptor_layout())... }
            , texture_descriptor_layouts{ static_cast<vk::DescriptorSetLayout>(
                  builders.get_texture_descriptor_layout())... }
        {

            const std::array<vk::GraphicsPipelineCreateInfo, Size> pipeline_create_infos{
                static_cast<vk::GraphicsPipelineCreateInfo>(builders.get_create_info())...
            };

            check(device.createGraphicsPipelines(
                VK_NULL_HANDLE,
                Size,
                pipeline_create_infos.data(),
                nullptr,
                pipelines.data())); // createGraphicsPipelines(VK_NULL_HANDLE, Size, pipeline_create_infos.data(),
                                   // nullptr, pipeline_layouts.data());
        }

    public:
        template<typename TShaderSet, pipeline_definition... Pipelines>
        static pipeline_collection<sizeof...(Pipelines)>
        make_pipeline_collection(vk::Device device,
                                 vk::RenderPass render_pass,
                                 vk::SampleCountFlagBits samples,
                                 TShaderSet const& shaders)
        {
            return pipeline_collection(
                device,
                Pipelines::initialize(
                    device, render_pass, samples, shaders.get_shaders(typename Pipelines::shader_list{}))...);
        }

        ~pipeline_collection()
        {

            for (auto texture_layout : texture_descriptor_layouts) {
                get_device().destroyDescriptorSetLayout(texture_layout);
            }

            for (auto uniform_layout : uniform_descriptor_layouts) {
                get_device().destroyDescriptorSetLayout(uniform_layout);
            }

            for (auto pipeline : pipelines) {
                get_device().destroyPipeline(pipeline);
            }

            for (auto layout : pipeline_layouts) {
                get_device().destroyPipelineLayout(layout);
            }
        }
    };
} // namespace obscure::vulkan