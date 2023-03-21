#ifndef OBSCURE_VULKAN_WINDOW_REFERENCE_DEFIONITION
#define OBSCURE_VULKAN_WINDOW_REFERENCE_DEFINITION 1

#include "glfw_vulkan_include.h"
#include <memory>
#include <tuple>

namespace obscure
{
	namespace vulkan
	{
		struct window_reference
		{
			std::shared_ptr<GLFWwindow> window_ptr;
			window_reference(int width, int height, const char* title);

			HWND get_handle() const&;

			void poll_events() const&;

			bool should_close() const&;

			VkExtent2D get_native_resolution() const&;
		};
	}
}

#endif