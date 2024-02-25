module;
#include <atomic>
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.glfw_window;

import obscure.internal.vulkan.glfw_api_context;

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	std::atomic_bool* resize_requested = reinterpret_cast<std::atomic_bool*>(glfwGetWindowUserPointer(window));
	if (resize_requested)
	{
		resize_requested->store(true);
	}
}

obscure::internal::vulkan::glfw_window::glfw_window(obscure::configuration::application_configuration* config)
{
	//Initialize glfw with the first call and clean up glfw when application exits
	static glfw_api_contex glfw_api_ctx;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


	glfwWindowHint(GLFW_RESIZABLE, (config->allow_resizing() ? GLFW_TRUE : GLFW_FALSE));


	window_ptr = glfwCreateWindow(config->window_width(), config->window_height(), config->application_title(), nullptr, nullptr);
}

obscure::internal::vulkan::glfw_window::glfw_window(glfw_window&& other) noexcept
	: window_ptr(other.window_ptr)
{
	other.window_ptr = nullptr;
}

obscure::internal::vulkan::glfw_window::glfw_window() noexcept
	: window_ptr(nullptr)
{

}

void obscure::internal::vulkan::glfw_window::enable_resizing(std::atomic_bool* resize_requested)
{
	glfwSetWindowUserPointer(window_ptr, resize_requested);
	glfwSetFramebufferSizeCallback(window_ptr, framebufferResizeCallback);
}

void obscure::internal::vulkan::glfw_window::free() noexcept
{
	if (window_ptr)
	{
		glfwDestroyWindow(window_ptr);
	}
}

HWND obscure::internal::vulkan::glfw_window::get_handle() const&
{
	return glfwGetWin32Window(window_ptr);
}

void obscure::internal::vulkan::glfw_window::poll_events() const&
{
	glfwPollEvents();
}

bool obscure::internal::vulkan::glfw_window::should_close() const&
{
	return glfwWindowShouldClose(window_ptr) == GLFW_TRUE;
}

VkExtent2D obscure::internal::vulkan::glfw_window::get_native_resolution() const&
{
	int width, height;
	glfwGetFramebufferSize(window_ptr, &width, &height);

	return VkExtent2D{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

bool obscure::internal::vulkan::glfw_window::is_minimized() const&
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window_ptr, &width, &height);

	return width == 0 || height == 0;
}