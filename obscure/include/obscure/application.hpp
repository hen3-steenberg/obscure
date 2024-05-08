#include "obscure/obscure_properties.hpp"
#ifndef OBSCURE_APPLICATION_DEFINITION
#define OBSCURE_APPLICATION_DEFINITION 1
#include "obscure/vulkan/instance.hpp"
#include "obscure/vulkan/logger.hpp"
#include "obscure/glfw/glfw_window.hpp"

namespace obscure
{
    struct application
    {
        obscure::glfw::glfw_window window;
        obscure::vulkan::instance vk_instance;
        obscure::vulkan::logger_collection<enable_debug_validation(), obscure::vulkan::verbose_console_logger, obscure::vulkan::verbose_json_logger> vk_loggers;


        application(const char * app_name, obscure::version app_version)
            : window(), vk_instance(app_name, app_version), vk_loggers()
        {}
    };
}

#endif