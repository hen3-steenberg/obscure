#ifndef OBSCURE_VULKAN_SURFACE_DEFINITION
#define OBSCURE_VULKAN_SURFACE_DEFINITION 1

#include "obscure/vulkan/instance.h"
#include "obscure/vulkan/window.h"

namespace obscure
{
	namespace vulkan
	{
		struct surface
		{
			VkSurfaceKHR vk_surface;
			surface() noexcept;
			surface(instance instance, glfw_window _window, VkAllocationCallbacks const* allocator = nullptr);
			VkSurfaceKHR get_handle() const noexcept;
			void free(instance inst, VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}
#endif