
#ifndef OBSCURE_APPLICATION_DEFINITION
#define OBSCURE_APPLICATION_DEFINITION 1
#include "obscure/obscure_properties.hpp"
#include "obscure/vulkan/instance.hpp"
#include "obscure/vulkan/logger.hpp"
#include "obscure/glfw/glfw_window.hpp"
#include "obscure/vulkan/device.hpp"
#include <functional>

namespace obscure
{



    struct application
    {
        obscure::glfw::glfw_window window;
        obscure::vulkan::instance vk_instance;
        obscure::vulkan::logger_collection<enable_debug_validation(), obscure::vulkan::verbose_console_logger, obscure::vulkan::verbose_json_logger> vk_loggers;
        obscure::vulkan::device vk_device;


        application(const char * app_name, obscure::version app_version, std::function<float (vk::PhysicalDevice)> get_device_score = vulkan::get_device_score_default)
            : window(), vk_instance(app_name, app_version), vk_loggers(), vk_device(vulkan::pick_device(vk_instance, std::move(get_device_score)))
        {}
    };
}

#endif