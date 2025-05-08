module;
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
export module surface;
import application_context;
export import glfw;

export namespace obscure::vulkan
{
        struct surface : vk::SurfaceKHR
        {
            static vk::Instance get_instance()
            {
                return obscure::get_application_instance();
            }

            surface(obscure::glfw::glfw_window_ref const& window)
                : vk::SurfaceKHR(window.create_surface(get_instance()))
            {}

            [[nodiscard]] vk::SurfaceKHR get_surface() const noexcept
            {
                return static_cast<vk::SurfaceKHR>(*this);
            }

            ~surface() noexcept
            {
                get_instance().destroySurfaceKHR(get_surface());
            }
        };
}