
#ifndef OBSCURE_APPLICATION_DEFINITION
#define OBSCURE_APPLICATION_DEFINITION 1

#include "obscure/glfw/glfw_window.hpp"
#include "obscure/vulkan/device.hpp"
#include "obscure/vulkan/surface.hpp"
#include "obscure/vulkan/swap_chain.hpp"
#include "obscure/vulkan/pipeline_definition.hpp"
#include "obscure/vulkan/shader_set.hpp"
#include "obscure/vulkan/pipeline_collection.hpp"
#include "obscure/vulkan/command_pool.hpp"
#include "obscure/vulkan/command_session.hpp"

#include <functional>

namespace obscure
{



    template<vulkan::pipeline_definition ... TPipelines>
    struct application
    {
#pragma region fields
        obscure::glfw::glfw_window window;
        obscure::vulkan::surface vk_surface;
        obscure::vulkan::device vk_device;
        using shader_set_t = vulkan::make_shader_set_t<TPipelines...>;
        shader_set_t shader_set;
        using swap_chain_t = obscure::vulkan::swap_chain;
        swap_chain_t vk_swap_chain;
        using pipeline_collection_t = obscure::vulkan::pipeline_collection<sizeof...(TPipelines)>;
        pipeline_collection_t vk_pipeline_collection;
        using command_pool_t = obscure::vulkan::command_pool;
        command_pool_t vk_command_pool;



        using command_session_t = obscure::vulkan::command_session<TPipelines...>;
#pragma endregion

#pragma region user_functions
        command_session_t begin_frame()
        {
            uint32_t frame_index = vk_swap_chain.get_next_frame_index();
            return command_session_t {vk_command_pool[vk_swap_chain.get_current_frame()], frame_index, vk_pipeline_collection.pipelines, vk_swap_chain};
        }

        void submit_frame()
        {
            constexpr static const vk::PipelineStageFlags wait_stages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

            vk::SubmitInfo submit_info {
                1,
                vk_swap_chain.get_image_available_ptr(),
                wait_stages,
                1,
                &vk_command_pool[vk_swap_chain.get_current_frame()],
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

#pragma endregion


        application(std::function<float (vk::PhysicalDevice)> get_device_score = vulkan::get_device_score_default)
            : window(), vk_surface(window),
            vk_device(vk_surface, std::move(get_device_score)),
            shader_set(vk_device.get_device()),
            vk_swap_chain(vk_device, vk_surface, window.get_window_ref()),
            vk_pipeline_collection( vulkan::pipeline_collection<sizeof...(TPipelines)>::template make_pipeline_collection<shader_set_t, TPipelines...>(vk_device.get_device(), vk_swap_chain.render_pass, shader_set)),
            vk_command_pool(vk_device, vk_swap_chain.get_frame_count())
        {
        }

        ~application()
        {
            vk_device.waitIdle();


        }
    };
}

#endif