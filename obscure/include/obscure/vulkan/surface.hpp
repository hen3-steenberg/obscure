#ifndef OBSCURE_VULKAN_SURFACE_DEFINITION
#define OBSCURE_VULKAN_SURFACE_DEFINITION
#include "glfw_vulkan_include.hpp"
#include "obscure/vulkan/instance.hpp"
#include "obscure/helper_templates/parent_reference.hpp"
#include "obscure/glfw/glfw_window.hpp"

namespace obscure
{
	namespace vulkan
	{
		template<typename ... Tsiblings>
		struct surface : vk::SurfaceKHR
		{
			obscure::vulkan::instance& get_parent_ref() &
			{
				return obscure::helper_templates::get_parent_ref<obscure::vulkan::instance, Tsiblings...>(this);
			}

			surface(obscure::glfw::glfw_window_ref const& window)
				: vk::SurfaceKHR(window.create_surface(get_parent_ref()))
			{}

			~surface() noexcept
			{
				get_parent_ref().destroySurfaceKHR(*this);
			}
		};
	}
}

#endif