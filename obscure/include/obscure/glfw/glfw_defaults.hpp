#ifndef OBSCURE_GLFW_DEFAULTS_DEFINITIONS
#define OBSCURE_GLFW_DEFAULTS_DEFINITIONS 1
#include "glfw_vulkan_include.hpp"

namespace obscure
{
	namespace glfw
	{
		[[nodiscard]] int get_fullscreen_height() noexcept;
		[[nodiscard]] int get_fullscreen_width() noexcept;

		[[nodiscard]] int get_default_window_height() noexcept;
		[[nodiscard]] int get_default_window_width() noexcept;
	}
}

#endif // !GLFW_DEFAULTS_DEFINITIONS
