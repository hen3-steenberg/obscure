#include "obscure/vulkan/window.h"
#include "obscure/vulkan/glfw_api_context.h"
#include "obscure/vulkan/application_context.h"


void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<obscure::vulkan::application_context*>(glfwGetWindowUserPointer(window));
	app->frame_buffer_resized = true;
}

obscure::vulkan::glfw_window::glfw_window(obscure::configuration::application_configuration* config, application_context* context)
{
	//Initialize glfw with the first call and clean up glfw when application exits
	static glfw_api_contex glfw_api_ctx;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


	glfwWindowHint(GLFW_RESIZABLE, (config->allow_resizing() ? GLFW_TRUE : GLFW_FALSE));


	window_ptr = glfwCreateWindow(config->window_width(), config->window_height(), config->application_title(), nullptr, nullptr);

	if (config->allow_resizing())
	{
		glfwSetWindowUserPointer(window_ptr, context);
		glfwSetFramebufferSizeCallback(window_ptr, framebufferResizeCallback);
	}
}

obscure::vulkan::glfw_window::glfw_window() noexcept
	: window_ptr(nullptr)
{

}

void obscure::vulkan::glfw_window::free() noexcept
{
	glfwDestroyWindow(window_ptr);
}

HWND obscure::vulkan::glfw_window::get_handle() const&
{
	return glfwGetWin32Window(window_ptr);
}

void obscure::vulkan::glfw_window::poll_events() const&
{
	glfwPollEvents();
}

bool obscure::vulkan::glfw_window::should_close() const&
{
	return glfwWindowShouldClose(window_ptr) == GLFW_TRUE;
}

VkExtent2D obscure::vulkan::glfw_window::get_native_resolution() const&
{
	int width, height;
	glfwGetFramebufferSize(window_ptr, &width, &height);

	return VkExtent2D{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

bool obscure::vulkan::glfw_window::is_minimized() const&
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window_ptr, &width, &height);

	return width == 0 || height == 0;
}