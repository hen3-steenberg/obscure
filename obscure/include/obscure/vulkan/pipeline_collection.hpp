#ifndef OBSCURE_VULKAN_PIPELINE_COLLECTION_DEFINITION
#define OBSCURE_VULKAN_PIPELINE_COLLECTION_DEFINITION 1
#include <array>
#include "obscure/vulkan/pipeline_definition.hpp"
#include "obscure/vulkan/device.hpp"
#include "obscure/helper_templates/parent_reference.hpp"
#include "glfw_vulkan_include.hpp"

namespace obscure {
    namespace vulkan {
        template<std::size_t Size>
        struct pipeline_collection_data
        {
            std::array<vk::Pipeline, Size> pipelines;
            std::array<vk::PipelineLayout, Size> pipeline_layouts;
        };

        template<pipeline_builder ... PipelineBuilders>
        pipeline_collection_data<sizeof...(PipelineBuilders)> make_pipeline_collection_impl(vk::Device device, PipelineBuilders ... builders)
        {
            constexpr uint32_t size = sizeof...(PipelineBuilders);
            std::array<vk::GraphicsPipelineCreateInfo, size> pipeline_create_infos {
                static_cast<vk::GraphicsPipelineCreateInfo>(builders.get_create_info())...
            };

            pipeline_collection_data<size> result;

            auto error = device.createGraphicsPipelines(VK_NULL_HANDLE, size, pipeline_create_infos.data(), nullptr, result.pipelines.data());

            result.pipeline_layouts = std::array { static_cast<vk::PipelineLayout>(builders.get_layout())... };

            return result;
        }

        template<typename TShaderSet, pipeline_definition ... Pipelines>
        pipeline_collection_data<sizeof...(Pipelines)> make_pipeline_collection(vk::Device device, vk::RenderPass render_pass, TShaderSet const& shaders)
        {
           return make_pipeline_collection_impl(device, Pipelines::initialize(device, render_pass, shaders.get_shaders( typename Pipelines::shader_list {} ))...);
        }

        template<std::size_t Size, typename ... TSiblings>
        struct pipeline_collection : pipeline_collection_data<Size>
        {
            private:
                device const& get_device_ref() const&
                {
                    return obscure::helper_templates::get_parent_ref<const device, TSiblings...>(this);
                }

            public:

                pipeline_collection(pipeline_collection_data<Size> && data)
                    : pipeline_collection_data<Size>(std::move(data))
                {}

                ~pipeline_collection()
                {
                    for(auto pipeline : pipeline_collection_data<Size>::pipelines)
                    {
                        get_device_ref().destroyPipeline(pipeline);
                    }

                    for(auto layout : pipeline_collection_data<Size>::pipeline_layouts)
                    {
                        get_device_ref().destroyPipelineLayout(layout);
                    }
                }
        };
    }
}
#endif