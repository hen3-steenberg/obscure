#ifndef OBSCURE_CONTEXT_DEFINITION
#define OBSCURE_CONTEXT_DEFINITION 1
#include "glfw_vulkan_include.hpp"
#include "obscure/utils/version.hpp"

namespace obscure
{
    void initialize(const char * app_name, obscure::version app_version);
    vk::Instance get_application_instance();
}
#endif