#ifndef OBSCURE_VULKAN_DEVICE_CONTEXT_DEFINITION
#define OBSCURE_VULKAN_DEVICE_CONTEXT_DEFINITION 1
#include "obscure/vulkan/device.hpp"
#include "obscure/vulkan/surface.hpp"
#include "obscure/glfw/glfw_window.hpp"
#include "obscure/vulkan/pipeline_definition.hpp"
#include "obscure/vulkan/shader_set.hpp"
#include "obscure/vulkan/swap_chain.hpp"
#include "obscure/vulkan/pipeline_collection.hpp"
#include "obscure/vulkan/command_pool.hpp"
#include "obscure/vulkan/command_session.hpp"
#include <functional>

namespace obscure::vulkan
{

    template<pipeline_definition ... TPipelines>
    struct device_context final
    {
        using shader_set_t = make_shader_set_t<TPipelines...>;
        using pipeline_collection_t = pipeline_collection<sizeof...(TPipelines)>;
        using command_session_t = obscure::vulkan::command_session<TPipelines...>;

        device vk_device;
        shader_set_t vk_shaders;
        swap_chain vk_swap_chain;
        pipeline_collection_t vk_pipeline_collection;
        command_pool vk_command_pool;

        device_context(surface const& _surface, glfw::glfw_window_ref _window, std::function<float (vk::PhysicalDevice)> get_device_score)
            : vk_device(_surface, std::move(get_device_score)),
              vk_shaders(vk_device.get_device()),
              vk_swap_chain(vk_device, _surface, _window),
              vk_pipeline_collection(pipeline_collection_t::template make_pipeline_collection<shader_set_t, TPipelines...>(vk_device.get_device(), vk_swap_chain.get_render_pass(), vk_shaders)),
              vk_command_pool(vk_device, vk_swap_chain.get_frame_count())
        {}

        ~device_context() noexcept {
            vk_device.waitIdle();
        }

        [[nodiscard]] vk::CommandBuffer get_current_buffer() const noexcept {
            return vk_command_pool[vk_swap_chain.get_current_frame()];
        }

        [[nodiscard]] vk::CommandBuffer const* get_current_buffer_ptr() const noexcept {
            return &vk_command_pool[vk_swap_chain.get_current_frame()];
        }

        command_session_t begin_frame()
        {
            uint32_t frame_index = vk_swap_chain.get_next_frame_index();
            return command_session_t {get_current_buffer(), frame_index, vk_pipeline_collection.pipelines, vk_swap_chain};
        }

        void submit_frame()
        {
            constexpr static const vk::PipelineStageFlags wait_stages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

            vk::SubmitInfo submit_info {
                1,
                vk_swap_chain.get_image_available_ptr(),
                wait_stages,
                1,
                get_current_buffer_ptr(),
                1,
                vk_swap_chain.get_render_finished_ptr()
            };

            auto _ = vk_device.get_graphics_queue().submit(1, &submit_info, vk_swap_chain.get_in_flight_fence());
        }

        void draw_frame()
        {
            vk::PresentInfoKHR present_info {
                1,
                vk_swap_chain.get_render_finished_ptr(),
                1,
                vk_swap_chain.get_swap_chain_ptr(),
                vk_swap_chain.get_frame_index_ptr()
            };

            auto _ = vk_device.get_present_queue().presentKHR(present_info);

        }


    };
}
#endif