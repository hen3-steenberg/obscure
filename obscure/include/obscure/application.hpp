
#ifndef OBSCURE_APPLICATION_DEFINITION
#define OBSCURE_APPLICATION_DEFINITION 1
#include "obscure/obscure_properties.hpp"
#include "obscure/vulkan/instance.hpp"
#include "obscure/vulkan/logger.hpp"
#include "obscure/glfw/glfw_window.hpp"
#include "obscure/vulkan/device.hpp"
#include "obscure/vulkan/surface.hpp"
#include "obscure/vulkan/swap_chain.hpp"
#include "obscure/vulkan/pipeline_definition.hpp"
#include "obscure/vulkan/shader_set.hpp"
#include "obscure/vulkan/pipeline_collection.hpp"
#include "obscure/vulkan/command_pool.hpp"
#include "obscure/vulkan/command_session.hpp"
#include "obscure/vulkan/semaphore.hpp"
#include "obscure/vulkan/fence.hpp"
#include "obscure/helper_templates/vector_initialize.hpp"
#include <functional>

namespace obscure
{

    inline std::vector<obscure::vulkan::semaphore> initialize_semaphores(std::size_t size, vk::Device device)
    {
        return obscure::helper_templates::initialize_vector<obscure::vulkan::semaphore>(size, device);
    }

    inline std::vector<obscure::vulkan::fence> initialize_fences(std::size_t size, vk::Device device, bool Signaled)
    {
        return obscure::helper_templates::initialize_vector<obscure::vulkan::fence>(size, device, Signaled);
    }

    template<vulkan::pipeline_definition ... TPipelines>
    struct application
    {
#pragma region fields
        obscure::glfw::glfw_window window;
        obscure::vulkan::instance vk_instance;
        using logger_t = obscure::vulkan::logger_collection<enable_debug_validation(), vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose, vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral, obscure::vulkan::console_logger, obscure::vulkan::json_logger>;
        logger_t vk_loggers;
        obscure::vulkan::surface<logger_t> vk_surface;
        obscure::vulkan::device vk_device;
        using shader_list = vulkan::make_shader_list_t<TPipelines...>;
        using shader_set_t = vulkan::make_shader_set_t<shader_list>;
        shader_set_t shader_set;
        using swap_chain_t = obscure::vulkan::swap_chain<shader_set_t>;
        swap_chain_t vk_swap_chain;
        using pipeline_collection_t = obscure::vulkan::pipeline_collection<sizeof...(TPipelines), shader_set_t, swap_chain_t>;
        pipeline_collection_t vk_pipeline_collection;
        using command_pool_t = obscure::vulkan::command_pool<shader_set_t, swap_chain_t, pipeline_collection_t>;
        command_pool_t vk_command_pool;

        std::vector<obscure::vulkan::semaphore> image_available;
        std::vector<obscure::vulkan::semaphore> render_finished;
        std::vector<obscure::vulkan::fence> in_flight;
        std::size_t current_frame;

        using command_session_t = obscure::vulkan::command_session<TPipelines...>;
#pragma endregion

#pragma region user_functions
        command_session_t begin_frame()
        {
            in_flight[current_frame].wait_and_reset();
            return command_session_t {vk_command_pool[current_frame], vk_swap_chain.get_next_frame_index(image_available[current_frame]), vk_pipeline_collection.pipelines, vk_swap_chain};
        }

        void submit_frame()
        {
            constexpr static const vk::PipelineStageFlags wait_stages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

            vk::SubmitInfo submit_info {
                1,
                image_available[current_frame].get_ptr(),
                wait_stages,
                1,
                &vk_command_pool[current_frame],
                1,
                render_finished[current_frame].get_ptr()
            };

            auto _ = vk_device.get_graphics_queue().submit(1, &submit_info, in_flight[current_frame].get());
        }

        void draw_frame()
        {
            vk::PresentInfoKHR present_info {
                1,
                render_finished[current_frame].get_ptr(),
                1,
                vk_swap_chain.get_ptr(),
                vk_swap_chain.get_frame_index_ptr()
            };

            auto _ = vk_device.get_present_queue().presentKHR(present_info);
            current_frame = (current_frame + 1) % vk_swap_chain.get_frame_count();
        }

#pragma endregion


        application(const char * app_name, obscure::version app_version, std::function<float (vk::PhysicalDevice)> get_device_score = vulkan::get_device_score_default)
            : window(), vk_instance(app_name, app_version), vk_loggers(), vk_surface(window),
            vk_device(vk_instance, vk_surface, std::move(get_device_score)),
            shader_set(),
            vk_swap_chain(vk_surface, window),
            vk_pipeline_collection(vulkan::make_pipeline_collection<shader_set_t, TPipelines...>(vk_device.get(), vk_swap_chain.render_pass, shader_set)),
            vk_command_pool(vk_swap_chain.get_frame_count()),
            image_available(initialize_semaphores(vk_swap_chain.get_frame_count(), vk_device.get())),
            render_finished(initialize_semaphores(vk_swap_chain.get_frame_count(), vk_device.get())),
            in_flight(initialize_fences(vk_swap_chain.get_frame_count(), vk_device.get(), true)),
            current_frame(0)
        {
        }

        ~application()
        {
                vk_device.waitIdle();
        }
    };
}

#endif