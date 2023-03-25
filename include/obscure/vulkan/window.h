#ifndef OBSCURE_VULKAN_WINDOW_DEFINITION
#define OBSCURE_VULKAN_WINDOW_DEFINITION 1

#include "obscure/vulkan/glfw_vulkan_include.h"

namespace obscure
{
	namespace vulkan
	{
		struct glfw_window
		{
			glfw_window() noexcept;
			GLFWwindow* window_ptr;
			glfw_window(int width, int height, const char* title);
			void free() noexcept;

			HWND get_handle() const&;

			void poll_events() const&;

			bool should_close() const&;

			VkExtent2D get_native_resolution() const&;
		};
	}
}

#endif