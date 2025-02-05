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

			vk::SurfaceKHR create_surface(vk::Instance inst) const
			{
				VkSurfaceKHR result;
				auto Err = glfwCreateWindowSurface(inst, window_ptr, nullptr, &result);
				if (Err != VK_SUCCESS)
				{
					throw std::runtime_error("Unable to create vulkan surface.");
				}
				return vk::SurfaceKHR{ result };
			}

			vk::Extent2D get_window_extent() const&
			{
				int width, height;
				glfwGetFramebufferSize(window_ptr, &width, &height);
				return vk::Extent2D{
					static_cast<uint32_t>(width),
					static_cast<uint32_t>(height)
				};
			}
		};

		struct glfw_window : glfw_window_ref
		{
			glfw_window();
			glfw_window(const glfw_window& other) = delete;
			glfw_window(glfw_window&& other);
			[[nodiscard]] glfw_window_ref get_window_ref() const noexcept;
			~glfw_window();
		};
	}
}

#endif