module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.surface;

import obscure.internal.vulkan.instance;
import obscure.internal.vulkan.glfw_window;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct surface
			{
				VkSurfaceKHR vk_surface;
				surface() noexcept;
				surface(instance instance, glfw_window _window, VkAllocationCallbacks const* allocator = nullptr);
				VkSurfaceKHR get_handle() const noexcept;
				void free(instance inst, VkAllocationCallbacks const* allocator = nullptr) noexcept;
			};
		}
	}
}