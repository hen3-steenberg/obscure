#ifndef OBSCURE_VULKAN_WINDOW_DEFINITION
#define OBSCURE_VULKAN_WINDOW_DEFINITION 1

#include "obscure/vulkan/glfw_vulkan_include.h"
#include "obscure/configuration/application_configuration.h"

namespace obscure
{
	namespace vulkan
	{
		struct application_context;

		struct glfw_window
		{
			glfw_window() noexcept;
			GLFWwindow* window_ptr;
			glfw_window(obscure::configuration::application_configuration* config, application_context* context );
			void free() noexcept;

			HWND get_handle() const&;

			void poll_events() const&;

			bool should_close() const&;

			bool is_minimized() const&;

			VkExtent2D get_native_resolution() const&;
		};
	}
}

#endif