
#ifndef OBSCURE_APPLICATION_DEFINITION
#define OBSCURE_APPLICATION_DEFINITION 1
#include "obscure/obscure_properties.hpp"
#include "obscure/vulkan/instance.hpp"
#include "obscure/vulkan/logger.hpp"
#include "obscure/glfw/glfw_window.hpp"
#include "obscure/vulkan/device.hpp"
#include "obscure/vulkan/surface.hpp"
#include "obscure/vulkan/swap_chain.hpp"
#include "obscure/vulkan/shader_module.hpp"
#include <functional>

namespace obscure
{



    struct application
    {
        obscure::glfw::glfw_window window;
        obscure::vulkan::instance vk_instance;
        using logger_t = obscure::vulkan::logger_collection<enable_debug_validation(), vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning, vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral, obscure::vulkan::console_logger, obscure::vulkan::json_logger>;
        logger_t vk_loggers;
        obscure::vulkan::surface<logger_t> vk_surface;
        obscure::vulkan::device vk_device;
        obscure::vulkan::swap_chain<> vk_swap_chain;



        application(const char * app_name, obscure::version app_version, std::function<float (vk::PhysicalDevice)> get_device_score = vulkan::get_device_score_default)
            : window(), vk_instance(app_name, app_version), vk_loggers(), vk_surface(window),
            vk_device(vk_instance, vk_surface, std::move(get_device_score)),
            vk_swap_chain(vk_surface, window)
        {
        }
    };

    static_assert(offsetof(application, vk_loggers) - offsetof(application, vk_instance) == sizeof(obscure::vulkan::instance));
}

#endif