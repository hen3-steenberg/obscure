module;
#include <cstdint>
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
#include <array>
export module obscure.properties;
export import obscure.utils.version;

export namespace obscure
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

    [[nodiscard]] consteval std::size_t max_image_count()
    {
        return 5;
    }
}