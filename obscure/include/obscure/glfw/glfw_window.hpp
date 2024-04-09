#ifndef OBSCURE_GLFW_WINDOW_DEFINITION
#define OBSCURE_GLFW_WINDOW_DEFINITION 1
#include "glfw_vulkan_include.hpp"

namespace obscure
{
	namespace glfw
	{
		struct glfw_window_ref
		{
			GLFWwindow* window_ptr;

			bool should_close() const
			{
				return glfwWindowShouldClose(window_ptr);
			}

			bool isKeyPressed(int KeyCode) const
			{
				return glfwGetKey(window_ptr, KeyCode) & (GLFW_PRESS | GLFW_RELEASE);
			}
		};

		struct glfw_window : glfw_window_ref
		{
			glfw_window();
			glfw_window(const glfw_window& other) = delete;
			glfw_window(glfw_window&& other);
			~glfw_window();
		};
	}
}

#endif