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
        struct pipeline_collection : protected virtual vk::Device
        {
            std::array<vk::Pipeline, Size> pipelines;
            std::array<vk::PipelineLayout, Size> pipeline_layouts;

        private:
            [[nodiscard]] vk::Device get_device() const
            {
                return static_cast<vk::Device>(*this);
            }

            template<pipeline_builder ... PipelineBuilders>
                requires (sizeof...(PipelineBuilders) == Size)
            explicit pipeline_collection(vk::Device device, PipelineBuilders ... builders)
                : vk::Device(device), pipeline_layouts{ static_cast<vk::PipelineLayout>(builders.get_layout())... } {

                const std::array<vk::GraphicsPipelineCreateInfo, Size> pipeline_create_infos {
                    static_cast<vk::GraphicsPipelineCreateInfo>(builders.get_create_info())...
                };

                auto error = device.createGraphicsPipelines(VK_NULL_HANDLE, Size, pipeline_create_infos.data(), nullptr, pipelines.data()); //createGraphicsPipelines(VK_NULL_HANDLE, Size, pipeline_create_infos.data(), nullptr, pipeline_layouts.data());
            }

        public:

            template<typename TShaderSet, pipeline_definition ... Pipelines>
            static pipeline_collection<sizeof...(Pipelines)> make_pipeline_collection(vk::Device device, vk::RenderPass render_pass, TShaderSet const& shaders) {
                return pipeline_collection(device, Pipelines::initialize(device, render_pass, shaders.get_shaders( typename Pipelines::shader_list {} ))...);
            }


            ~pipeline_collection()
            {
                for(auto pipeline : pipelines)
                {
                    get_device().destroyPipeline(pipeline);
                }

                for(auto layout : pipeline_layouts)
                {
                    get_device().destroyPipelineLayout(layout);
                }
            }
        };
    }
}
#endif