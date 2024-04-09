#include "obscure/glfw/glfw_defaults.hpp"
#include "obscure/glfw/glfw_window.hpp"

obscure::glfw::glfw_window::glfw_window()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window_ptr = glfwCreateWindow(get_default_window_width(), get_default_window_height(), "", glfwGetPrimaryMonitor(), nullptr);
}

obscure::glfw::glfw_window::glfw_window(glfw_window&& other)
	: glfw_window_ref(other)
{
	other.window_ptr = nullptr;
}

obscure::glfw::glfw_window::~glfw_window()
{
	if (window_ptr)
	{
		glfwDestroyWindow(window_ptr);
	}
}