#include "obscure/vulkan/window_reference.h"
#include "obscure/vulkan/glfw_api_context.h"


obscure::vulkan::window_reference::window_reference(int width, int height, const char* title)
{
	//Initialize glfw with the first call and clean up glfw when application exits
	static glfw_api_contex glfw_api_ctx;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window_ptr.reset(glfwCreateWindow(width, height, title, nullptr, nullptr), glfwDestroyWindow);
}

HWND obscure::vulkan::window_reference::get_handle() const&
{
	return glfwGetWin32Window(window_ptr.get());
}

void obscure::vulkan::window_reference::poll_events() const&
{
	glfwPollEvents();
}

bool obscure::vulkan::window_reference::should_close() const&
{
	return glfwWindowShouldClose(window_ptr.get()) == GLFW_TRUE;
}