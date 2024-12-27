
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
#include <functional>

namespace obscure
{


    template<vulkan::pipeline_definition ... TPipelines>
    struct application
    {
        obscure::glfw::glfw_window window;
        obscure::vulkan::instance vk_instance;
        using logger_t = obscure::vulkan::logger_collection<enable_debug_validation(), vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning, vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral, obscure::vulkan::console_logger, obscure::vulkan::json_logger>;
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



        application(const char * app_name, obscure::version app_version, std::function<float (vk::PhysicalDevice)> get_device_score = vulkan::get_device_score_default)
            : window(), vk_instance(app_name, app_version), vk_loggers(), vk_surface(window),
            vk_device(vk_instance, vk_surface, std::move(get_device_score)),
            shader_set(),
            vk_swap_chain(vk_surface, window),
            vk_pipeline_collection(vulkan::make_pipeline_collection<shader_set_t, TPipelines...>(vk_device.get(), vk_swap_chain.render_pass, shader_set))
        {
        }
    };
}

#endif