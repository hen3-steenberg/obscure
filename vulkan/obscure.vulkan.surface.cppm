module;
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.surface;
import obscure.vulkan.application_context;
export import obscure.glfw;

export namespace obscure::vulkan
{
        struct surface : vk::SurfaceKHR
        {
            static vk::Instance get_instance()
            {
                return obscure::get_application_instance();
            }

            explicit surface(obscure::glfw::glfw_window_ref const& window)
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