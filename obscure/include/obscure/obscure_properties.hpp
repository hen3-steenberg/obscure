#ifndef OBSCURE_PROPERTY_DEFINITIONS
#define OBSCURE_PROPERTY_DEFINITIONS 1
#include "obscure/utils/version.hpp"
#include "glfw_vulkan_include.hpp"
#include <array>

namespace obscure
{
    [[nodiscard]] consteval version obscure_version()
	{
		return version {1, 0, 0};
	}

    [[nodiscard]] consteval bool enable_debug_validation()
    {
        #ifdef DEBUG
            return true;
        #else
            return false;
        #endif
    }

    [[nodiscard]] consteval const char* obscure_name()
    {
        return "OBSCURE";
    }

    [[nodiscard]] consteval auto required_device_extensions()
    {
        return std::array<const char*, 1> { "VK_KHR_swapchain" };
    }

    [[nodiscard]] consteval vk::ClearColorValue get_clear_color()
    {
        return {0.0f, 0.0f, 0.0f, 1.0f};
    }
}
#endif