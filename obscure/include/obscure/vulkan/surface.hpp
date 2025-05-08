#ifndef OBSCURE_VULKAN_SURFACE_DEFINITION
#define OBSCURE_VULKAN_SURFACE_DEFINITION
#include "obscure/application_context.hpp"
#include "obscure/helper_templates/parent_reference.hpp"
#include "obscure/glfw/glfw_window.hpp"
#include "obscure/obscure_properties.hpp"

namespace obscure
{
	namespace vulkan
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
}

#endif