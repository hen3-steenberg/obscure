#include "obscure/vulkan/window.h"
#include "obscure/vulkan/glfw_api_context.h"


obscure::vulkan::glfw_window::glfw_window(int width, int height, const char* title)
{
	//Initialize glfw with the first call and clean up glfw when application exits
	static glfw_api_contex glfw_api_ctx;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window_ptr = glfwCreateWindow(width, height, title, nullptr, nullptr);
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