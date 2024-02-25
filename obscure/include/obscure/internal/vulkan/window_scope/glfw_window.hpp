module;
#include "glfw_vulkan_include.h"
#include <atomic>
export module obscure.internal.vulkan.glfw_window;

import obscure.configuration.application_configuration;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			
			export struct glfw_window
			{
				glfw_window() noexcept;
				GLFWwindow* window_ptr;
				glfw_window(obscure::configuration::application_configuration* config);
				glfw_window(glfw_window&& other) noexcept;
				void free() noexcept;

				void enable_resizing(std::atomic_bool* resize_requested)&;

				HWND get_handle() const&;

				void poll_events() const&;

				bool should_close() const&;

				bool is_minimized() const&;

				VkExtent2D get_native_resolution() const&;
			};
		}
	}
}